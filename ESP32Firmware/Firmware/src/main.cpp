#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WiFi.h>

// Wifi Credentials
const String wifi_ssid = "wifi ssid";         // network name
const String wifi_password = "wifi passowrd"; // network passowrd

// Websocket Details
const String websocket_server = "websocket server"; // ex: 192.---.-.---
const int websocket_port = 8080;                    // websocket port
const String websocket_path = "/ws";                // websocket path
WebSocketsClient webSocket;

// websocket event
// checks websocket status
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
  case WStype_CONNECTED:
    Serial.println("[WebSocket] Connected");
    break;
  case WStype_DISCONNECTED:
    Serial.println("[WebSocket] Disconnected");
    break;
  case WStype_TEXT:
    Serial.printf("[WebSocket] Received text: %s\n", payload);
    break;
  default:
    break;
  }
}

// webSocket setup code
// calls websocket event to post status on serial
void setUpWebSocket() {
  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

// wifi setup code
// outputs wifi setup status messages to serial
void setUpWifi() {
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.println("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected!");
}

void setup() {
  // setup serial
  Serial.begin(115200);

  // connect to wifi
  setUpWifi();

  // setup the websocket
  setUpWebSocket();
}

void loop() {}
