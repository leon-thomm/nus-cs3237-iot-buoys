#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

namespace wifi {

    const char* ssid = "AndroidAP"; //Your Wifi's SSID
    const char* password = "bsnu4874"; //Wifi Password

    WiFiClient client;
    // const char* laptopAt = "http://192.168.43.1/";
    IPAddress phone(192,168,43,1);
    // WeMos IP: 192.168.43.33 (??)
    // laptop IP: 192.168.43.156 (??)

    void init(){
        WiFi.begin(ssid, password);

        // Wait for connection
        Serial.print("establishing wifi connection");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }

    bool sendJSON(StaticJsonDocument<200> doc) {
        char* buffer = (char*)malloc(sizeof(char) * 200);
        if (client.connect(phone, 4567)) {
            Serial.println("connected");
            serializeJson(doc, buffer, 200);
            client.print(buffer);
        }
        return true;
    
    }

}