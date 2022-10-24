#ifndef ADC
#define ADC

#include <Wire.h>

namespace adc_manual {

    /*
    
    README

    Initially, I was using this manual approach, however I soon discovered that this does
    not work reliably. When I try to read from A0, it reads from A1

    */

    #define MAX_ACCESS_RATE_MS 500

    byte address;
    int last_update = 0;
    double read_buffer[4] = {0.0, 0.0, 0.0, 0.0};

    void read_all();

    void init(byte i2c_address = 0x48) {
        address = i2c_address;

        // Serial.println("ADC: reading initial sensor data from all channels...");
        // read_all();
        // Serial.println("ADC: done");
    }

    void wait() {
        int t = millis();
        if(t - last_update < MAX_ACCESS_RATE_MS) {
            delay(MAX_ACCESS_RATE_MS - (t - last_update));
        }
    }

    float read(int channel) {
        
        if (millis() - last_update < MAX_ACCESS_RATE_MS) {
            Serial.println("ADC: WARNING: you are reading too fast! returning 0.0");
            return 0.0;
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

        Serial.print("for channel ");
        Serial.print(channel);
        Serial.print(": ");
        Serial.println(0b10000000 | (adc_inp << 4), BIN);


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
                float ret = regValue / 17550.0;
                // TODO: why does the ADC give such a weird number, whereas analogRead()
                //  returns much better results?
                return ret;
            }
        }
    }

    void read_all() {
        for(int c=0; c<4; c++) {
            read_buffer[c] = read(c);
            delay(MAX_ACCESS_RATE_MS);
        }
    }
}


#include <ADS1115_WE.h>

namespace adc {

    byte address;
    ADS1115_WE* _adc;

    void init(byte i2c_address = 0x48) {
        address = i2c_address;
        _adc = new ADS1115_WE(address);
        if(!_adc->init()){
            Serial.println("ADS1115 not connected!");
        }
        _adc->setVoltageRange_mV(ADS1115_RANGE_4096);
    }

    float read(int channel) {
        float voltage = 0.0;
        ADS1115_MUX c;
        switch (channel) {
            case 0: c = ADS1115_COMP_0_GND; break;
            case 1: c = ADS1115_COMP_1_GND; break;
            case 2: c = ADS1115_COMP_2_GND; break;
            case 3: c = ADS1115_COMP_3_GND; break;
            default: Serial.println("ADC: ERROR: invalid channel"); return 0.0;
        }
        _adc->setCompareChannels(c);
        _adc->startSingleMeasurement();
        while(_adc->isBusy()){}
        voltage = _adc->getResult_V();
        return voltage / 3.3;
    }

}

#endif