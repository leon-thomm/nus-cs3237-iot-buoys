#include <ESP8266WiFi.h>
#include <PubSubClient.h>


namespace mqtt {
    
    // const char *mqtt_broker = "iot.ckteo.com";
    IPAddress server(104,248,98,70);
    const char *mqtt_uname = "cs3237";
    const char *mqtt_pw = "thisisag00dp4ssw0rd";
    const int mqtt_port = 1883;
    PubSubClient client;

    namespace wakeup {
        
        const char *topic = "test/wakeup";
        void (*wakeup_cb)() = nullptr;

        void callback(char *topic_, byte *payload, unsigned int length) {
            Serial.print("Message arrived in topic: ");
            Serial.println(topic);
            Serial.print("Message:");

            if (strcmp(topic_, topic)) { wakeup_cb(); }

            Serial.println();
        }

        void init(WiFiClient& wifi_client, void (*wakeup_cb_)())
        {
            wakeup_cb = wakeup_cb_;
            
            client = PubSubClient(server, mqtt_port, callback, wifi_client);

            String client_id = "esp8266-client-";
            client_id += String(WiFi.macAddress());
            
            while (!client.connected()) {
                Serial.print("connecting to mqtt...");
                if (client.connect(client_id.c_str(), mqtt_uname, mqtt_pw)) {
                    Serial.println("connected");
                    client.publish("test/wakeup", "hello world");
                    client.subscribe("tets/wakeup");
                } else {
                    Serial.println(client.state());
                    Serial.println("retry in 2 seconds");
                    delay(2000);
                }
            }

        }

        void publish_wakeup() {
            client.publish(topic, "bro shit's getting real!");
        }

        void loop() {
            client.loop();
        }
    
    }

}