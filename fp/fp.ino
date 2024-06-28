#include <WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>

// Update these with your WiFi and MQTT broker settings
const char* ssid = "masihspotpanas";
const char* password = "mbokarep";
const char* mqtt_server = "152.42.194.14";
const char* mqtt_pub_topic = "/iot06/box/c2647bb2-d87a-4489-a0db-5d7840abb36a/pub-sensor";
const char* mqtt_sub_topic = "/iot06/box/c2647bb2-d87a-4489-a0db-5d7840abb36a/sub-access";

WiFiClient espClient;
PubSubClient client(espClient);

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

const int pirPin = 13;  // PIR sensor pin
const int vibrationPin = 12;  // Vibration sensor pin
const int solenoidPin = 27; // Solenoid pin

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {23, 22, 21, 19}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {18, 5, 4, 2}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables to track keypad input and state
String keypadInput = "";
bool isCollectingInput = false;
bool solenoidState = false; // To track the solenoid state

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 22, 23);  // RX, TX pins for GPS module
  
  pinMode(pirPin, INPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(solenoidPin, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  digitalWrite(solenoidPin, LOW);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0'; // Null-terminate the payload
  Serial.println((char*)payload);

  if (strcmp(topic, mqtt_sub_topic) == 0) {
    if (strcmp((char*)payload, "1") == 0) {
      solenoidState = true;
    } else if (strcmp((char*)payload, "0") == 0) {
      solenoidState = false;
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Create JSON object
  StaticJsonDocument<256> doc;
  doc["pir"] = digitalRead(pirPin) == HIGH ? "Motion" : "Freeze";
  doc["vibration"] = digitalRead(vibrationPin) == HIGH ? "Vibrate" : "Idle";

  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
    if (gps.location.isUpdated()) {
      doc["gps"]["latitude"] = gps.location.lat();
      doc["gps"]["longitude"] = gps.location.lng();
    }
  }

  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      // Start collecting input
      isCollectingInput = true;
      keypadInput = ""; // Reset input string
    } else if (key == '#') {
      // Stop collecting input and send data
      isCollectingInput = false;
      doc["keypad"] = keypadInput;

      // Serialize JSON to string
      char jsonBuffer[512];
      serializeJson(doc, jsonBuffer);

      // Publish JSON string to MQTT topic
      client.publish(mqtt_pub_topic, jsonBuffer);

      // Reset keypad input
      keypadInput = "";
    } else if (isCollectingInput) {
      // Collect keypad input
      keypadInput += key;
    }
  }

  if (!isCollectingInput) {
    doc["keypad"] = "null";
    doc["flag"] = "NO";

    // Serialize JSON to string
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publish JSON string to MQTT topic
    client.publish(mqtt_pub_topic, jsonBuffer);
  }

  // Update solenoid pin based on solenoidState
  if (solenoidState) {
    digitalWrite(solenoidPin, HIGH);
  } else {
    digitalWrite(solenoidPin, LOW);
  }

  delay(3000);  // adjust delay as necessary
}