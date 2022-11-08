#include <ArduinoJson.h>

#include "i2c/i2c.h"
#include "adc/adc.h"
#include "photores/photores.h"
#include "temperature/temperature.h"
#include "mpu/mpu.h"
#include "wifi/wifi.h"
#include "scheduler/scheduler.h"
#include "mqtt/mqtt.h"

#define MPU_ADDR 0x68
#define ADC_ADDR 0x48

int adc_address;
int timestamp_offset;
bool mqtt_enabled = false;

void setup()
{
    Serial.begin(9600);
    while (!Serial) continue;
    delay(2000);

    // initialize I2C
    Wire.begin();
    //   make sure we find both I2C devices
    Serial.println("scanning for I2C devices...");
    int* dev = i2c::scan(false);
    Serial.println("found the following devices:");
    for(int i=0; i<dev[0]; i++) {
        Serial.print("device ");
        Serial.print(i+1);
        Serial.print(" at address ");
        Serial.print(dev[i+1]);
        Serial.println();
    }

    while(dev[0] != 2){}

    // initialize sensor interfaces
    adc::init(ADC_ADDR);
    photores::init(0);
    temperature::init(1);
    mpu::init(MPU_ADDR);

    // initialize wifi
    timestamp_offset = wifi::init();

    // initialize scheduler
    scheduler::init();

    // initialize mqtt wakeup
    if (mqtt_enabled) {
        mqtt::wakeup::init(wifi::client, scheduler::set_intense);
    }

    // turn on light to indicate successful setup
    photores::turn_light_on();
    
}

void loop()
{

    scheduler::wait();

    // check mqtt
    if (mqtt_enabled) {
        mqtt::wakeup::loop();
    }

    if(!scheduler::intense) photores::turn_light_on();

    // read adc
    float brightness = photores::read();
    float heat = temperature::read();

    // read mpu
    
    xyzFloat acc = mpu::getG();
    xyzFloat gyr = mpu::getGyr();
    float res_g = mpu::getResG();

    // generate json doc
    
    StaticJsonDocument<200> doc;
    doc["time"] = millis() - timestamp_offset;
    doc["temp"] = heat;
    doc["light"] = brightness;
    JsonArray acc_doc = doc.createNestedArray("acc");
    acc_doc.add(acc.x);
    acc_doc.add(acc.y);
    acc_doc.add(acc.z);
    acc_doc.add(res_g);
    JsonArray gyr_doc = doc.createNestedArray("gyro");
    gyr_doc.add(gyr.x);
    gyr_doc.add(gyr.y);
    gyr_doc.add(gyr.z);
    serializeJson(doc, Serial);
    Serial.println();

    wifi::sendJSON(doc);

    scheduler::update(res_g, brightness);

    if(!scheduler::intense) photores::turn_light_off();

}