#include <WiFi.h>
#include <WebServer.h>

// Create Access Point credentials
const char* ssid = "ESP32_HOTSPOT";
const char* password = "12345678";

// Create web server on port 80
WebServer server(80);

// LED pin (use onboard LED)
const int ledPin = 2;

// Function to blink LED
void blinkLED() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
}

void moveLeft() {
  Serial.println("Moving car to left...");
  return;
}

void moveRight() {
  Serial.println("Moving car to right...");
  return;
}

void moveForward() {
  Serial.println("Moving car to forward...");
  return;
}

void moveBackward() {
  Serial.println("Moving car to backward...");
  return;
}


// HTML webpage
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Car Control</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: #111;
    color: white;
    text-align: center;
    margin-top: 50px;
  }
  h2 {
    color: #0ff;
  }
  button {
    width: 120px;
    height: 60px;
    font-size: 20px;
    margin: 10px;
    border-radius: 10px;
    border: none;
    cursor: pointer;
    color: white;
    background: #444;
  }
  button:hover {
    background: #0ff;
    color: black;
  }
</style>
</head>
<body>
  <h2>ESP32 Control Panel</h2>
  <div>
    <button onclick="sendCmd('F')">Forward</button>
  </div>
  <div>
    <button onclick="sendCmd('L')">Left</button>
    <button onclick="sendCmd('R')">Right</button>
  </div>
  <div>
    <button onclick="sendCmd('B')">Backward</button>
  </div>

<script>
function sendCmd(cmd) {
  fetch('/cmd?move=' + cmd);
}
</script>
</body>
</html>
)rawliteral";

// Handle root URL
void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleUserInput(String UserMove) {
  char move = UserMove[0];  // Get the first character directly

  switch (move) {
    case 'L':
      moveLeft();
      break;
    case 'R':
      moveRight();
      break;
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
  }
}

// Handle command URL
void handleCommand() {
  String move = server.arg("move");
  Serial.print("Received command: ");
  Serial.println(move);
  handleUserInput(move);

  blinkLED();

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  Serial.println("Setting up Access Point...");

  // Start hotspot
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Define server routes
  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);

  // Start server
  server.begin();
  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}