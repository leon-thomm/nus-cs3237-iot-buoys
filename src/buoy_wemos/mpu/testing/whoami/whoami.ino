#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

void setup() {
    delay(5000);
    byte whoAmICode = 0x00;
    Serial.begin(9600);
    Serial.println("Starting MPU9250");
    Wire.begin();
    myMPU9250.init();
    Serial.println("initialized");

    whoAmICode = myMPU9250.whoAmI();
    Serial.print("WhoAmI Register: 0x");
    Serial.println(whoAmICode, HEX);
    switch(whoAmICode){
    case(0x70):
        Serial.println("Your device is an MPU6500.");
        Serial.println("The MPU6500 does not have a magnetometer."); 
        break;
    case(0x71):
        Serial.println("Your device is an MPU9250");
        break;
    case(0x73):
        Serial.println("Your device is an MPU9255");
        Serial.println("Not sure if it works with this library, just try");
        break;
    case(0x75):
        Serial.println("Your device is probably an MPU6515"); 
        Serial.println("Not sure if it works with this library, just try");
        break;
    case(0x00):
        Serial.println("Can't connect to your device. Check all connections.");
        break;
    default:
        Serial.println("Unknown device - it may work with this library or not, just try"); 
    }  
}

void loop() {
}