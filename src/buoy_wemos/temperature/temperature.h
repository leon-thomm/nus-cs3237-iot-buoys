#include "../adc/adc.h"

namespace temperature {
    // #define TEMP_PIN A0
    #define TEMP_VOLAGE 3.3

    int adc_channel;

    // https://arduinomodules.info/ky-013-analog-temperature-sensor-module/

    float R1 = 10000; // value of R1 on board
    float logR2, R2, T;
    float c1 = 0.001129148, 
        c2 = 0.000234125, 
        c3 = 0.0000000876741;
    //steinhart-hart coeficients for thermistor

    void init(int adc_ch) {
        adc_channel = adc_ch;
        // pinMode(TEMP_PIN, INPUT);
    }

    float read() {
        int v = adc::read(adc_channel) * 1024.0;
        // int v = analogRead(TEMP_PIN);

        R2 = R1 * (1023.0 / (float)v - 1.0); // calculate resistance on thermistor
        logR2 = log(R2);
        T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
        T = T - 273.15; // convert Kelvin to Celcius
        // T = (T * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit
        return T;
    }
}

/*
    example:

void setup()
{
  Serial.begin(9600);
  initialize_temp();
}
 
void loop()
{
    Serial.print("Temperature:\t\t"); 
    Serial.println(read_temp());

    delay(1000);                                     //waiting a second
}
*/
