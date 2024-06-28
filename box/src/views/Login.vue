<template>
    <div class="login-container">
      <div class="login-box">
        <h2>Login</h2>
        <form @submit.prevent="handleLogin">
          <div class="form-group">
            <label for="username">Username:</label>
            <input type="text" id="username" v-model="username" required>
          </div>
          <div class="form-group">
            <label for="password">Password:</label>
            <input type="password" id="password" v-model="password" required>
          </div>
          <button type="submit" class="login-button">Login</button>
        </form>
      </div>
    </div>
  </template>
  
  <script>
  import { ref } from 'vue';
  import { useRouter } from 'vue-router';
  
  export default {
    name: 'Login',
    setup() {
      const router = useRouter();
      const username = ref('');
      const password = ref('');
  
      const handleLogin = () => {
        if (username.value && password.value) {
          if (username.value === 'admin' && password.value === 'admin') {
            router.push('/home');
          } else {
            alert('Invalid username or password');
          }
        }
      };
  
      return {
        username,
        password,
        handleLogin,
      };
    },
  };
  </script>
  
  <style scoped>
  .login-container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    background: linear-gradient(135deg, #f3f4f7, #e2e2e2);
  }
  
  .login-box {
    background: white;
    padding: 2em;
    border-radius: 8px;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    width: 100%;
    max-width: 400px;
    text-align: center;
  }
  
  h2 {
    margin-bottom: 1em;
    color: #333;
  }
  
  .form-group {
    margin-bottom: 1em;
    text-align: left;
  }
  
  label {
    display: block;
    margin-bottom: 0.5em;
    color: #555;
  }
  
  input[type="text"],
  input[type="password"] {
    width: 100%;
    padding: 0.75em;
    border: 1px solid #ccc;
    border-radius: 4px;
    font-size: 1em;
  }
  
  input[type="text"]:focus,
  input[type="password"]:focus {
    border-color: #007bff;
    outline: none;
    box-shadow: 0 0 5px rgba(0, 123, 255, 0.25);
  }
  
  .login-button {
    display: inline-block;
    width: 100%;
    padding: 0.75em;
    background-color: #007bff;
    border: none;
    border-radius: 4px;
    color: white;
    font-size: 1em;
    cursor: pointer;
  }
  
  .login-button:hover {
    background-color: #0056b3;
  }
  
  .login-button:focus {
    outline: none;
    box-shadow: 0 0 5px rgba(0, 123, 255, 0.5);
  }
  </style>
  