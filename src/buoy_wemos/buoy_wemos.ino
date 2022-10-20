#include "i2c/i2c.h"
#include "adc/adc.h"
#include "photores/photores.h"
#include "temperature/temperature.h"
// #include "mpu/mpu.h"

int adc_address;


void setup()
{
    Wire.begin();
    Serial.begin(9600);

    // scan
    Serial.println("scanning for I2C devices...");
    int* dev = i2c::scan(false);
    Serial.println("done");

    Serial.println("found the following devices:");
    for(int i=0; i<dev[0]; i++) {
        Serial.print("device ");
        Serial.print(i+1);
        Serial.print(" at address ");
        Serial.print(dev[i+1]);
        Serial.println();
    }

    // TODO: how to ensure the ADC's address is the first when using multiple I2C devices?
    adc_address = dev[1];

    // initialize sensor interfaces
    adc::init(adc_address);
    photores::init(0);
    // temperature::init(1);
    // mpu::init();
}

void loop()
{
    // read ADC channel 0 (photo resistor)
    Serial.print("photores:\t\t");
    Serial.println(photores::read());
    // Serial.print("temperature:\t\t");
    // Serial.println(temperature::read());
    delay(1000);

    /*  mpu

    xyzFloat gValue = mpu.getGValues();
    xyzFloat gyr = mpu.getGyrValues();
    float temp = mpu.getTemperature();
    float resultantG = mpu.getResultantG(gValue);

    Serial.print("acc [g] x,y,z,g:\t\t");
    Serial.print(gValue.x);
    Serial.print("\t\t");
    Serial.print(gValue.y);
    Serial.print("\t\t");
    Serial.print(gValue.z);
    Serial.print("\t\t");
    Serial.println(resultantG);

    Serial.print("gyro [deg/s] x,y,z:\t\t");
    Serial.print(gyr.x);
    Serial.print("\t\t");
    Serial.print(gyr.y);
    Serial.print("\t\t");
    Serial.println(gyr.z);

    */
}