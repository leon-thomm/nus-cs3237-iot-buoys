#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

namespace wifi {

    const char* ssid = "iotbuoy";
    const char* password = "abcd1234";

    WiFiClient client;
    IPAddress phone(192,168,43,1);
    // IPAddress phone(192,168,250,244);

    bool sendString(char* buf) {
        if (client.connect(phone, 4567)) {
            Serial.println("connected");
            Serial.print("sending: ");
            Serial.println(buf);

            client.print(buf);
        }
        return true;
    }

    bool sendJSON(StaticJsonDocument<200> doc) {
        char* buffer = (char*)malloc(sizeof(char) * 200);
        serializeJson(doc, buffer, 200);
        return sendString(buffer);
    }

    long init(){
        WiFi.mode(WIFI_STA);    // possibly required for mqtt
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

        sendString("reset");
        return millis();
    }

}