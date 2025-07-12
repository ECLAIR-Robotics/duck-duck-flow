#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

// Wifi Credentials
const String wifi_ssid = "wifi ssid";         // network name
const String wifi_password = "wifi passowrd"; // network passowrd

// Websocket Details
const String websocket_server = "websocket server"; // ex: 192.---.-.---
const int websocket_port = 8080;                    // websocket port
const String websocket_path = "/ws";                // websocket path
WebSocketsClient webSocket;



void setup() {
}

void loop() {
}
