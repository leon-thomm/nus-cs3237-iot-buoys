#include "mpu/mpu.h"

void setup() {
	Serial.begin(9600);
	Wire.begin();
	delay(2000);

	initialize_mpu();
	Serial.println("done.");
	delay(2000);
}

void loop() {
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

  Serial.print("Temperature [°C]: ");
  Serial.println(temp);

  Serial.println("********************************************");

  delay(1000);
}