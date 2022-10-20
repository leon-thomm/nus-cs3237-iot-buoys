#ifndef ADC
#define ADC

#include <Wire.h>

namespace adc {

    #define MAX_ACCESS_RATE_MS 100

    byte address;
    int last_update = 0;

    void init(byte i2c_address = 0x48) {
        address = i2c_address;
    }

    float read(int channel) {
        // the has a slow access rate, if we access too frequently it gives garbage values
        int t = millis();
        if(t - last_update < MAX_ACCESS_RATE_MS) {
            delay(MAX_ACCESS_RATE_MS - (t - last_update));
        }
        last_update = millis();

        // channel: 0-3
        byte adc_inp;   // configures the ADC input multiplexer
        switch (channel) {
            case 0: adc_inp = 0b100; break;
            case 1: adc_inp = 0b101; break;
            case 2: adc_inp = 0b110; break;
            case 3: adc_inp = 0b111; break;
            default: return 0;
        }

        Wire.beginTransmission(address);
        int error = Wire.endTransmission();

        if (error) {
        }else {

            Wire.beginTransmission(address);
            Wire.write(0b00000001); // Config Register
            Wire.write(0b10000000 | (adc_inp << 4)); // MSB of Config Register
            Wire.write(0b10000011); // LSB of Config Register
            Wire.endTransmission();

            Wire.beginTransmission(address);
            Wire.write(0b00000000); // Conversion Register
            Wire.endTransmission();

            Wire.requestFrom(address,2);
            if(Wire.available()){

                uint8_t MSByte = Wire.read();
                uint8_t LSByte = Wire.read();
                uint16_t regValue = (MSByte<<8) + LSByte;
                return regValue / 17550.0;
                // TODO: why does the ADC give such a weird number, whereas analogRead()
                //  returns much better results?
            }
        }
    }
}

#endif