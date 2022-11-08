#include "../adc/adc.h"

namespace photores {

    #define LED D5
    
    int Brightness = 255;
    int adc_channel;

    void turn_light_on()
    {
        analogWrite(LED, Brightness);
    }

    void turn_light_off() 
    {
        analogWrite(LED, 0);
    }

    float read() 
    {
        // returns the current brightness as a relative value from 0.0 - 1.0
        return adc::read(adc_channel);
    }

    void init(int adc_ch) 
    {
        adc_channel = adc_ch;
        pinMode(LED, OUTPUT);
        turn_light_off();
    }

}


/*

    SETUP

    - connect S to +3.3V
    - connect middle to GND
    - connect - to analog in

*/