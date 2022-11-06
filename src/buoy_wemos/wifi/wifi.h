#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

namespace wifi {

    const char* ssid = "iotbuoy"; //Your Wifi's SSID
    const char* password = "abcd1234"; //Wifi Password

    WiFiClient client;
    IPAddress phone(192,168,43,1);      // leon
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

        // Serial.println("waiting for timestamp...");
        // long timestamp = 0;
        // char* buffer = (char*)calloc(sizeof(char), 20);
        // int buffer_idx = 0;
        // if (client.connect(phone, 4567)) {

        //     while(client.read() == 255){}
        //     while(true) {
        //         delay(100);
        //         char c = client.read();
        //         Serial.print("received char: ");
        //         Serial.write(c);
        //         Serial.println((int)c);
        //         if(c == 255) { break; }
        //         buffer[buffer_idx++] = c;
        //     }
        //     Serial.print("received timestamp buffer: ");
        //     Serial.println(buffer);
        //     Serial.print("as string: ");
        //     Serial.println(String(buffer));
        //     Serial.println("converting to long: ");
        //     timestamp = String(buffer).toInt();
        //     Serial.print("received timestamp: ");
        //     Serial.println(timestamp);
        // }

        // return timestamp;
    }

}