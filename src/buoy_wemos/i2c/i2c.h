#ifndef I2C
#define I2D

#include <Wire.h>

namespace i2c {
    #define MAX_I2C_DEVICES 127

    int* scan(bool print_info=false) {
        // returns n int list whose first element denotes the number of devices found,
        // and whose subsequent elements are the addresses

        int nDevices = 0;
        int* addresses = (int*)calloc(MAX_I2C_DEVICES+1, sizeof(int));

        byte error, address;
        if (print_info) Serial.println("Scanning...");
        for(address = 1; address < MAX_I2C_DEVICES; address++) {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();
            
            if (error == 0) {
                if (print_info) {
                    Serial.println("I2C device found at address 0x");
                    if (address<16) {
                        Serial.print("0");
                    }
                    Serial.print(address, HEX);
                    Serial.println(" !");
                }
                nDevices++;
                addresses[nDevices] = address;
            }

        }
        if (print_info) {
            if (nDevices == 0) {
                Serial.println("No I2C devices found");
            }else{
                Serial.println("done");
            }
        }
        addresses[0] = nDevices;
        return addresses;
    }
}


/*
    example

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    // scan
    Serial.println("scanning for devices...");
    int* dev = scan_i2c_devices();
    Serial.println("done");

    Serial.println("found the following devices:");
    for(int i=0; i<dev[0]; i++) {
        Serial.print("device ");
        Serial.print(i+1);
        Serial.print(" at address ");
        Serial.print(dev[i+1]);
        Serial.println();
    }
}

*/

#endif