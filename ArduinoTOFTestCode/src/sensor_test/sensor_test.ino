#include <Arduino_JSON.h>

// sensor_test.ino
// 10-2-2025
// test program for ultrasonic sensor to print out values. originally compiled and run in arduino IDE

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

String path = "https://tanayhome.org/log";

const char* ssid     = "utexas-iot";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "DUMMY";     // The password of the Wi-Fi network

SoftwareSerial mySerial(D5, D6); // TX green d5, RX blue d6 // according to AI, these are good chioces for pins for NodeMCU
// we need software serial 
unsigned char data[4]={};
float distance;
void setup()
{
  mySerial.begin(9600);   // match ESP8266's baud rate
  Serial.begin(9600); // match ESP8266's baud rate

  delay(10);
  Serial.println('\n');

  WiFi.begin(ssid, password);
  // Connect to network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.print(WiFi.localIP());
}

void loop()
{
  // Serial.print("checkpoint 1");
    do {
     for(int i=0;i<4;i++) {
      data[i]=mySerial.read();
    }
  } while (mySerial.read() == 0xff);
  // Serial.printf("checkpoint 2 data= %d|%d|%d|%d|\n", data[0], data[1], data[2], data[3]);
  // Serial.printf("checkpoint 2 data = %02X|%02X|%02X|%02X|\n", data[0], data[1], data[2], data[3]);
  mySerial.flush();
  if(data[0] == 0xff) {
      int sum;
      sum=(data[0] + data[1] + data[2]) & 0x00FF;
      if (sum==data[3]) {
        distance=(data[1]<<8)+data[2];
        if(distance > 280) {
          Serial.print("distance=");
          Serial.print(distance / 10);
          Serial.println("cm");
          if (WiFi.status() == WL_CONNECTED){
            auto client = std::make_unique<BearSSL::WiFiClientSecure>();
            client->setInsecure();

            HTTPClient https;

            // Create JSON object
            JSONVar myData;
            myData["water_height"] = distance / 10;
            myData["latitude"] = 40.786;
            myData["longitude"] = -74.006;
            myData["timestamp"] = "2025-10-06T19:03:08.000Z";
            myData["sensor_altitude"] = 0.5;
            myData["comment"] = "Test from ESP";
            myData["sensor_id"] = 69;
            
            // Serialize to JSON string
            String jsonString = JSON.stringify(myData);
            Serial.println("JSON to send:");
            Serial.println(jsonString);
            
            // Your Domain name with URL path or IP address with path
            https.begin(*client, "https://tanayhome.org/api/log");
            https.addHeader("Content-Type", "application/json");
              
            // Send HTTP POST request
            int httpResponseCode = https.POST(jsonString);
            
            if (httpResponseCode > 0) {
              Serial.print("HTTP Response code: ");
              Serial.println(httpResponseCode);
              String payload = https.getString();
              Serial.println(payload);
            }
            else {
              Serial.print("Error code: ");
              Serial.println(httpResponseCode);
            }
            // Free resources
            https.end();
          } else {
            Serial.println("WiFi Disconnected");
          }
        } else {
          Serial.println("Below the lower limit");         
        }
      } else Serial.println("ERROR: checksum failed");
     }
     delay(30000);
}