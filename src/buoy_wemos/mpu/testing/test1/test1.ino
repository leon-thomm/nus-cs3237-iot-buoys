#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>

MPU9250 mpu; // You can also use MPU9255 as is
void print(char* msg) {
    Serial.println(msg);
}

void setup() {
    print("booting");
    Serial.begin(9600);
    Wire.begin();
    print("wire open");

    delay(2000);
    
    print("calibrating mpu");
    mpu.setup(0x68);  // change to your own address
    // mpu.calibrateAccelGyro();
    // mpu.calibrateMag();
    print("setup done.");
}

// the loop function runs over and over again forever
void loop() {
    print("looping");
    scan_i2c();

    // if (mpu.update()) {
    //     Serial.print(mpu.getYaw()); Serial.print(", ");
    //     Serial.print(mpu.getPitch()); Serial.print(", ");
    //     Serial.println(mpu.getRoll());
    // }
    delay(1000);
}

int* scan_i2c() {
    int nDevices = 0;
    int* devices = (int*)malloc(128 * sizeof(int));
    Serial.println("Scanning...");
    for (int i = 1; i < 127; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("I2C device found at address 0x");
            if (i < 16) {
                Serial.print("0");
            }
            Serial.print(i, HEX);
            Serial.println(" !");
            devices[nDevices] = i;
            nDevices++;
        }
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found");
    } else {
        Serial.println("done");
    }
    if (nDevices < 128) {
        devices[nDevices] = -1;  // mark end of array
    }
    return devices;
}