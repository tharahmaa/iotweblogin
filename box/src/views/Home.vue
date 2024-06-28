<template>
  <div class="home">
    <h2>Home</h2>
    <button @click="addBox">Add Box</button>
    <div v-for="(box, index) in boxes" :key="index" class="box">
      <p>Box {{ index + 1 }}</p>
      <p>Location: {{ box.location }}</p>
      <p>Movement detected: {{ box.movementDetected ? 'Yes' : 'No' }}</p>
    </div>
  </div>
</template>

<script>
import { reactive } from 'vue';

export default {
  name: 'Home',
  setup() {
    const boxes = reactive([]);

    const addBox = () => {
      boxes.push({
        location: `Location ${boxes.length + 1}`,
        movementDetected: Math.random() > 0.5,
      });
    };

    return {
      boxes,
      addBox,
    };
  },
};
</script>

<style scoped>
.home {
  max-width: 600px;
  margin: 0 auto;
}
.box {
  border: 1px solid #ccc;
  padding: 1em;
  margin-top: 1em;
  border-radius: 5px;
}
</style>
