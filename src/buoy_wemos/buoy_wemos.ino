#include <ArduinoJson.h>

#include "i2c/i2c.h"
#include "adc/adc.h"
#include "photores/photores.h"
#include "temperature/temperature.h"
#include "mpu/mpu.h"
#include "wifi/wifi.h"

#define MPU_ADDR 0x68
#define ADC_ADDR 0x48   // hardwired
#define FETCH_BASE_INT_MS 1000

int adc_address;
int last_fetch;
long timestamp_offset;

void wait() {
    int t = millis();
    if(t - last_fetch < FETCH_BASE_INT_MS) {
        delay(FETCH_BASE_INT_MS - (t - last_fetch));
    }
    last_fetch = millis();
}

void setup()
{
    Serial.begin(9600);
    while (!Serial) continue;
    delay(2000);

    // I2C

    Wire.begin();
    // scan: make sure we find both I2C devices
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

    // wifi
    timestamp_offset = wifi::init();
}

void loop()
{

    wait();

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

    // print

    // Serial.print("photores:\t\t");
    // Serial.println(brightness);
    // Serial.print("temperature:\t\t");
    // Serial.print(heat);
    // Serial.println();

    // Serial.print("acc [g] x,y,z,g:\t\t");
    // Serial.print(gValue.x);
    // Serial.print("\t\t");
    // Serial.print(gValue.y);
    // Serial.print("\t\t");
    // Serial.print(gValue.z);
    // Serial.print("\t\t");
    // Serial.println(resultantG);

    // Serial.print("gyro [deg/s] x,y,z:\t\t");
    // Serial.print(gyr.x);
    // Serial.print("\t\t");
    // Serial.print(gyr.y);
    // Serial.print("\t\t");
    // Serial.println(gyr.z);

    delay(1000);
}