console.log("ESP32C3 Web Interface Loaded");

function fetchConfig() {
    fetch('/getledconfig')
        .then(response => response.json())
        .then(data => {
            console.log("Config loaded:", data);
        });
}

window.onload = fetchConfig;
