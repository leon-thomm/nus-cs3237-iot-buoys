#include <MPU9250.h>
#include <Wire.h>

MPU9250 mpu; // You can also use MPU9255 as is

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(2000);

    mpu.setup(0x68);  // change to your own address
}

void loop() {
    if (mpu.update()) {
        Serial.println("Hello, World!");
//        Serial.print(mpu.getYaw()); Serial.print(", ");
//        Serial.print(mpu.getPitch()); Serial.print(", ");
//        Serial.println(mpu.getRoll());
    }
}
