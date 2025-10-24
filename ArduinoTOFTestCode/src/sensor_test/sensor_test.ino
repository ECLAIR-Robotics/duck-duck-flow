// sensor_test.ino debug version
// 10-20-2025
// test program for ultrasonic sensor to print out values. originally compiled and run in arduino IDE
/**
 * The ultrasonicsensor will take an average of distance measurements in cm 
 * over 30 seconds (see LOOP_LIMIT), then send the data to the DuckDuckFlow 
 * testing database.
 */

#define DEBUG 1  // print out debug statements
#define DONT_SEND_DATA 0 // whether data should be sent to the database

#include <Arduino_JSON.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "WifiConnection.h" // not pushed. see WifiConnection.txt
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h> // For breaking down epoch time

// Time variables
static unsigned long lastSync = 0;
static const unsigned long syncInterval = 300000; // 5 minutes
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // UTC time, update every 60 seconds

// loop variables
#define SENSOR_DELAY 150      // don't make it too high or the sensor won't work
#define LOOP_LIMIT 30         // the number of loops to take the measurement average over. LOOP_LIMIT * SENSOR_DELAY should be around 30,000 = 30 seconds
static int iterCount = 0;     // which iteration of the loop we're on, will be reset after sending data
static int goodIterCount = 0;     // number of successful runs, for taking average
static float runningSum = 0;  // running sum, divide by LOOP_LIMIT to get average
static bool canSend = false;  // whether we can send data or not yet, every LOOP_LIMIT-th iteration

// connection variables (logging)
const String path = "https://tanayhome.org/log";
const char* ssid     = WIFI_NAME;         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = WIFI_PASSWORD;     // The password of the Wi-Fi network

// measurement variables
// we need software serial 
SoftwareSerial mySerial(D5, D6); // TX green d5, RX blue d6 // according to AI, these are good chioces for pins for NodeMCU
unsigned char data[4]={};
float distance; // from ultrasonic sensor. to water level

void setup() {
  mySerial.begin(9600);   // match ESP8266's baud rate
  Serial.begin(9600);     // match ESP8266's baud rate

  delay(5000);
  // print constants
  if (DEBUG) {
    Serial.println("DEBUG mode is ON!");
  } else {
    Serial.println("Debug mode is off");
  }
  if (DONT_SEND_DATA) {
    Serial.println("DONT_SEND_DATA is TRUE. Data will NOT be sent to the database!");
  }

  // Connect to network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("Waiting...");

  // wait to connect to network.
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }
  
  // successfully connected
  Serial.println();
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // setup time
  timeClient.begin();
  // Wait until NTP time is available
  while (!timeClient.update()) {
    timeClient.forceUpdate(); // Try to fetch time immediately
    delay(100);
  }
  setTime(timeClient.getEpochTime()); 
}

void loop() {
  // Sync time every 5 minutes
  if (millis() - lastSync > syncInterval) {
    timeClient.update();
    setTime(timeClient.getEpochTime());
    lastSync = millis();
  }

  // Take measurement
  float distancetoSend = getDistance(); // your measurement function

  // check for errors
  if (distancetoSend > 0) {
    // update variables for average
    runningSum += distancetoSend;
    ++goodIterCount;

    // this block generates TOO many prints - keep for later just in case
    // Serial.print("running sum += ");
    // Serial.print(distancetoSend);
    // Serial.print(" = ");
    // Serial.print(runningSum);
    // Serial.print(". number of valid mesurements =");
    // Serial.print(goodIterCount);
  
    // Get timestamp
    char isoTime[25];
    sprintf(isoTime, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            year(), month(), day(), hour(), minute(), second());
  
    // Send data if certain conditions are met
    attemptSendData(isoTime);
  }

  delay(SENSOR_DELAY);  // MUST wait before taking another measurement
}

/* returns the current distance in cm */
float distanceInCm() {
  return distance / 10;
}

/** reads data from sensor and returns the distance in cm. 
 * must have a delay between readings */
float getDistance() {
  float result = -1;

  // wait for data to arrive
  do {
    for(int i=0;i<4;i++) {
      data[i]=mySerial.read();
    }
  } while (mySerial.read() == 0xff);
  
  // check for valid data
  mySerial.flush();
  if(data[0] == 0xff) {
    // checksum: did the data send correctly?
    int sum;
    sum = (data[0] + data[1] + data[2]) & 0x00FF;
    if (sum==data[3]) {
      // check below lower limit (too close)
      distance = (data[1]<<8)+data[2];
      if(distance > 280) {
        return distanceInCm();
      } else {
        Serial.println("Below the lower limit");         
      }
    } else Serial.println("ERROR: checksum failed");
  } // else { Serial.println("no data"); }

  // if (DEBUG) {
  //   Serial.print("getDistance returning result=");
  //   Serial.println(result);
  // }
  return result;
}

/** sends data if certain conditions are met */
void attemptSendData(String isotimestamp) {
  // send data every LOOP_LIMIT iterations. have a flag since the next few loops might fail.
  if (!canSend && ++iterCount > LOOP_LIMIT) {
    canSend = true; 
    Serial.println("Reached required loops, can send data now!");
  } else if (iterCount % 10 == 0) {
    // this most recent measurement
    Serial.print("just measured ");
    Serial.print(distanceInCm());
    Serial.print("cm at ");
    Serial.println(isotimestamp);

    Serial.print("taken ");
    Serial.print(iterCount);
    Serial.print("/");
    Serial.print(LOOP_LIMIT);
    Serial.println(" measurements until next send");
  }

  if (canSend) {
    // send data
    sendData(isotimestamp);
    // reset average variables
    canSend = false; // reset can send flag
    iterCount = 0; // reset iterCount
    goodIterCount = 0;
    runningSum = 0.0; //reset
  }
}

/** sends data to path */
void sendData(String isotimestamp) {
  // send if conditions met
  if (WiFi.status() == WL_CONNECTED) {
    float average = runningSum / goodIterCount;

    // Create JSON object
    JSONVar myData;
    myData["water_height"] = average;
    myData["latitude"] = 40.786;
    myData["longitude"] = -74.006;
    myData["timestamp"] = isotimestamp;  // "2025-10-06T19:03:08.000Z";
    myData["sensor_altitude"] = 0.5;
    myData["comment"] = "Test from ESP testing timestamp";
    myData["sensor_id"] = 69;
    
    // Serialize to JSON string
    String jsonString = JSON.stringify(myData);

    if (DONT_SEND_DATA) {
      Serial.println("NOT ACTUALLY SENDING JSON:");
      Serial.println(jsonString);
    } else {
      // setup connection
      auto client = std::make_unique<BearSSL::WiFiClientSecure>();
      client->setInsecure();
      HTTPClient https;

      // send data
      https.begin(*client, "https://tanayhome.org/api/log");
      https.addHeader("Content-Type", "application/json");
      
      // Send HTTP POST request
      Serial.println("Sending data:");
      Serial.println(jsonString);
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
    }
    
  } else {
    Serial.println("WiFi Disconnected");
  }
} 
