# 10-9-2025 Notes

Who was here? Isabella, Robert, Shalini

# What did we do?

Connect ESp 8266 to wifi!! And sucessfully insert data to the database through the api/log endpoint

## Install Arduino json
Go to sketch>include libraries>manage libraries
Arduino_JSON by Arduino

## API endpoint log code
code to [sucessfully log data!](../ArduinoTOFTestCode/src/sensor_test/sensor_test.ino)

## Test WiFi Connection
For reference
[Cameron's connecting ESP32 to utexas-iot wifi video](https://drive.google.com/file/d/1cpV1IbiYtUqx8-zFBPuiBPHAlqy9cxQQ/view)
[Guide referred to in the above video](https://docs.google.com/document/d/1wwSIg8m34-DuAw-DcoKp99E60ic94Iq9u-OrUdoP69Q)
[How to print out MAC address for this ESP8266](<10-6-2025 notes.md>)
```cpp
#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid     = "utexas-iot";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "ASK ISABELLA FOR PASSWORD";     // The password of the Wi-Fi network

void readMacAddress() {
  uint8_t baseMac[6];
  WiFi.macAddress(baseMac);

  Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                baseMac[0], baseMac[1], baseMac[2],
                baseMac[3], baseMac[4], baseMac[5]);
}

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(8000); // long delay because it was too fast last time
  Serial.println('\n');
  delay(2000);

// double check mac address for not randomization
  WiFi.mode(WIFI_STA);
  Serial.print("[DEFAULT] ESP8266 Board MAC Address: ");
  readMacAddress();
  Serial.print("expected mac address: c8:2b:96:09:94:55\n");
  
  WiFi.begin(ssid, password);             // Connect to the network
  delay(2000);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}



void loop() { }
```

# next time
make password a constant, don't push to github
