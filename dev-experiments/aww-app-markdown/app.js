// on document ready or if already loaded and ready vanilla js

const appDiv = document.getElementById('app');

setInterval(() => {
  appDiv.innerHTML = new Date().toString();
}, 1000);


