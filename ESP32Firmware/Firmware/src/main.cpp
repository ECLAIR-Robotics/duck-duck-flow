#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>

// Wifi Credentials
const String wifi_ssid = "wifi ssid";         // network name
const String wifi_password = "wifi passowrd"; // network passowrd

// Websocket data
const String websocket_url =
    "wss://duck-duck-flow.tanay-garg.workers.dev"; // url of websocket
using namespace websockets;
WebsocketsClient client; // define the websockets client

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

// setup the websocket
void setUpWebSocket() {
  // Setup WebSocket callbacks
  client.onMessage([](WebsocketsMessage message) {
    Serial.print("Received message: ");
    Serial.println(message.data());
  });

  // check websocket event
  client.onEvent([](WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
      Serial.println("WebSocket connection opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
      Serial.println("WebSocket connection closed");
    } else if (event == WebsocketsEvent::GotPing) {
      Serial.println("Received ping");
    } else if (event == WebsocketsEvent::GotPong) {
      Serial.println("Received pong");
    }
  });

  // Connect to WebSocket server
  Serial.println("Connecting to WebSocket server...");
  bool connected = client.connect(websocket_url);

  if (connected) {
    Serial.println("WebSocket connected successfully!");

    // Send a test message
    client.send("Hello from ESP32!");

  } else {
    Serial.println("Failed to connect to WebSocket server");
  }
}

void setup() {
  // setup serial
  Serial.begin(115200);

  // connect to wifi
  setUpWifi();

  // setup websocket
  setUpWebSocket();
}

// makes sure device connected to websocket
void heartBeat() {
  // Keep the WebSocket connection alive
  client.poll();

  // Send periodic messages (optional)
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) { // Send every 10 seconds
    if (client.available()) {
      String message = "ESP32 heartbeat: " + String(millis());
      client.send(message);
      Serial.println("Sent: " + message);
    }
    lastSend = millis();
  }
}

// handle disconection and reconnection
void checkConnection() {
  if (!client.available()) {
    Serial.println("WebSocket disconnected, attempting to reconnect...");
    delay(5000);

    bool reconnected = client.connect(websocket_url);
    if (reconnected) {
      Serial.println("WebSocket reconnected successfully!");
    } else {
      Serial.println("Failed to reconnect to WebSocket server");
    }
  }

  delay(100); // Small delay to prevent watchdog issues
}

void loop() {
  // baseline funny connection infastructure stuffs
  // dont mess with this cuz everything will die!!!
  heartBeat();
  checkConnection();
}
