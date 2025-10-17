#include <Arduino_JSON.h>

// sensor_test.ino debug version
// 10-2-2025
// test program for ultrasonic sensor to print out values. originally compiled and run in arduino IDE

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "WifiConnection.h" // not pushed. see WifiConnection.txt

// loop variables
#define SENSOR_DELAY 150      // don't make it too high or the sensor won't work
#define LOOP_LIMIT 200         // the number of loops to take the measurement average over. LOOP_LIMIT * SENSOR_DELAY should be around 30,000 = 30 seconds
static int iterCount = 0;     // which iteration of the loop we're on, will be reset after sending data
static int goodIterCount = 0;     // number of successful runs, for taking average
static float runningSum = 0;  // running sum, divide by LOOP_LIMIT to get average
static bool canSend = false;  // whether we can send data or not yet, every LOOP_LIMIT-th iteration

const String path = "https://tanayhome.org/log";

const char* ssid     = WIFI_NAME;         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = WIFI_PASSWORD;     // The password of the Wi-Fi network

SoftwareSerial mySerial(D5, D6); // TX green d5, RX blue d6 // according to AI, these are good chioces for pins for NodeMCU
// we need software serial 
unsigned char data[4]={};
float distance;
void setup()
{
  mySerial.begin(9600);   // match ESP8266's baud rate
  Serial.begin(9600);     // match ESP8266's baud rate

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
  // send data every LOOP_LIMIT iterations. have a flag since the next few loops might fail.
  if (!canSend && ++iterCount > LOOP_LIMIT) {
    canSend = true; 
    Serial.println("Reached required loops, can send data now!");
  }

  do {
    // read in data
    for(int i=0;i<4;i++) {
      data[i]=mySerial.read();
    }
  } while (mySerial.read() == 0xff);
  
  // check for data
  mySerial.flush();
  if(data[0] == 0xff) {
    // checksum: did the data send correctly?
    int sum;
    sum = (data[0] + data[1] + data[2]) & 0x00FF;
    if (sum==data[3]) {
      // check below lower limit
      distance = (data[1]<<8)+data[2];
      if(distance > 280) {
        float distanceInCm = distance / 10;
        Serial.print("distance=");
        Serial.print(distanceInCm);
        Serial.println("cm");

        // update variables for average
        runningSum += distanceInCm;
        ++goodIterCount;

        // send if conditions met
        if (canSend) {
          if (WiFi.status() == WL_CONNECTED) {
            
            // auto client = std::make_unique<BearSSL::WiFiClientSecure>();
            // client->setInsecure();

            // HTTPClient https;

            float average = runningSum / goodIterCount;

            // Create JSON object
            JSONVar myData;
            myData["water_height"] = average;
            myData["latitude"] = 40.786;
            myData["longitude"] = -74.006;
            myData["timestamp"] = "2025-10-06T19:03:08.000Z";
            myData["sensor_altitude"] = 0.5;
            myData["comment"] = "Test from ESP";
            myData["sensor_id"] = 69;
            
            // Serialize to JSON string
            String jsonString = JSON.stringify(myData);
            Serial.println("NOT ACTUALLY SENDING JSON:");
            Serial.println(jsonString);
            
            // // Your Domain name with URL path or IP address with path
            // https.begin(*client, "https://tanayhome.org/api/log");
            // https.addHeader("Content-Type", "application/json");
              
            // // Send HTTP POST request
            // int httpResponseCode = https.POST(jsonString);
            
            // if (httpResponseCode > 0) {
            //   Serial.print("HTTP Response code: ");
            //   Serial.println(httpResponseCode);
            //   String payload = https.getString();
            //   Serial.println(payload);
            // }
            // else {
            //   Serial.print("Error code: ");
            //   Serial.println(httpResponseCode);
            // }
            // // Free resources
            // https.end();

            // reset average variables
            canSend = false; // reset can send flag
            iterCount = 0; // reset iterCount
            runningSum = 0.0; //reset
          } else {
            Serial.println("WiFi Disconnected");
          }
        } // else can't send yet
      } else {
        Serial.println("Below the lower limit");         
      }
    } else Serial.println("ERROR: checksum failed");
  } // else { Serial.println("no data"); }

  // wait before taking another measurement
  delay(SENSOR_DELAY);
}