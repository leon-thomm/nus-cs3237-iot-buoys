#include "../adc/adc.h"

namespace photores {

    int adc_channel;

    void init(int adc_ch) {
        adc_channel = adc_ch;
    }

    float read() {
        // returns the current brightness as a relative value from 0.0 - 1.0
        return adc::read(adc_channel);
    }
}


/*

    SETUP

    - connect S to +3.3V
    - connect middle to GND
    - connect - to analog in

*/