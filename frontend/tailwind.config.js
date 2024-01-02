module.exports = {
  mode : 'jit',
  purge : ['./src/**/*.svelte'],

  theme: {
    extend: {
      colors:{
        jet: "#333333",
        skyblue: "#74C7D3",
        sunglow: "#FECB2E",
        prussianBlue: "00384D"
      }
    },
  },
};