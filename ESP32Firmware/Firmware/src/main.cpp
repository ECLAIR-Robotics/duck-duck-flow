#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>

const char* ssid = "utexas-iot";
const char* password = "90638287949629994713";
const IPAddress remote_ip(173, 194, 208, 139); // Google's public DNS server
int port = 80; // HTTP port

WiFiClient client = WiFiClient();

void setup(){
    Serial.begin(115200);
    Serial.print("\nDefault ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());
    WiFi.begin(ssid, password);
    

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi network");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("ESP32 Hostname: ");
    Serial.println(WiFi.getHostname());

    // Ping google.com to check connectivity
    if (Ping.ping(remote_ip)) {
        Serial.println("Successfully pinged remote IP: " + remote_ip.toString());
    } else {
        Serial.println("Ping failed");
    }

    // HTTP POST request example
    HttpClient http(client);

    // send a get request to https://reqbin.com/echo/get/json
    char hostname[] = "reqbin.com";
    IPAddress ip = IPAddress(0, 0, 0, 0);
    WiFi.hostByName(hostname, ip);
    Serial.println("Resolved hostname: " + String(hostname) + " to IP: " + ip.toString());
    http.get(ip, NULL, "/echo/get/json", NULL);

    if (http.responseStatusCode() == 200) {
        Serial.println("HTTP GET request successful");
        char response[512];
        http.readBytes(response, sizeof(response) - 1);
        response[sizeof(response) - 1] = '\0'; // Null-terminate the string
        Serial.println("Response: " + String(response));
    } else {
        Serial.println("HTTP GET request failed with status code: " + String(http.responseStatusCode()));
    }


}
 
void loop(){
  // Do Nothing
}