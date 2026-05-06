#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#define CONFIG_FREERTOS_UNICORE 1

const int ENA = 20;
const int IN1 = 19;
const int IN2 = 18;
const int IN3 = 7;
const int IN4 = 6;
const int ENB = 5;

const int freq = 20000;
const int resolution = 8;
const int dutyCycle = 255;

const char* ssid = "Auto_S3_Competencia_cristobal";
const char* password = "password123";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Control Lab 2</title>
    <style>
        body { font-family: Arial; background: #1a1a1a; color: white; text-align: center; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; margin: 0; overflow: hidden; }
        .grid { display: grid; grid-template-columns: repeat(3, 100px); grid-template-rows: repeat(3, 100px); gap: 10px; }
        button { width: 100px; height: 100px; background: #3498db; border: none; border-radius: 10px; color: white; font-size: 2em; touch-action: manipulation; }
        button:active { background: #2980b9; }
        .stop { background: #e74c3c; }
        #up { grid-column: 2; }
        #left { grid-column: 1; grid-row: 2; }
        #stop { grid-column: 2; grid-row: 2; }
        #right { grid-column: 3; grid-row: 2; }
        #down { grid-column: 2; grid-row: 3; }
        .status { margin-bottom: 20px; padding: 10px; border-radius: 5px; background: #444; }
    </style>
</head>
<body>
    <div id="stat" class="status">Desconectado</div>
    <div class="grid">
        <button id="up" ontouchstart="s('F')" ontouchend="s('S')" onmousedown="s('F')" onmouseup="s('S')">↑</button>
        <button id="left" ontouchstart="s('L')" ontouchend="s('S')" onmousedown="s('L')" onmouseup="s('S')">←</button>
        <button id="stop" class="stop" onclick="s('S')">■</button>
        <button id="right" ontouchstart="s('R')" ontouchend="s('S')" onmousedown="s('R')" onmouseup="s('S')">→</button>
        <button id="down" ontouchstart="s('B')" ontouchend="s('S')" onmousedown="s('B')" onmouseup="s('S')">↓</button>
    </div>
    <script>
        var w;
        function init() {
            w = new WebSocket('ws://' + window.location.hostname + '/ws');
            w.onopen = () => { document.getElementById('stat').innerHTML = "Conectado"; };
            w.onclose = () => { setTimeout(init, 2000); };
        }
        function s(c) { if(w.readyState == 1) w.send(c); }
        window.onload = init;
    </script>
</body>
</html>
)rawliteral";

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void handleMessage(void *arg, uint8_t *data, size_t len) {
  if (len > 0) {
    char cmd = (char)data[0];
    
    if (cmd == 'F') {
      turnRight();
    } 
    else if (cmd == 'L') {
      moveBackward();
    } 
    else if (cmd == 'B') {
      turnLeft();
    } 
    else if (cmd == 'R') {
      moveForward();
    } 
    else if (cmd == 'S') {
      stopMotors();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) handleMessage(arg, data, len);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);

  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);

  ledcWrite(0, dutyCycle);
  ledcWrite(1, dutyCycle);

  stopMotors();

  WiFi.softAP(ssid, password);
  Serial.println("Servidor iniciado.");
  Serial.println(WiFi.softAPIP());

  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
}