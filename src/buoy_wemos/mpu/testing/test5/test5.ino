#include<Wire.h>

#define MPU_ADDR 0x68

void setup()
{
    Serial.begin(9600);
    delay(5000);
    Serial.println("Starting MPU9250");
    Wire.begin();
    Wire.setClock(400000UL); // Set I2C frequency to 400kHz
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0); // wake up the mpu6050
    Wire.endTransmission(true);
    Serial.println("MPU9250 Started");
}
void loop() {            
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR,6,true);  // request a total of 6 registers 

    double AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    double AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    double AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" \t\t| AcY = "); Serial.print(AcY);
    Serial.print("\t\t | AcZ = "); Serial.println(AcZ);

    // delay(1000);
}

// double Gyro_X()
// {
//     Wire.beginTransmission(mpu_addr);
//     Wire.write(0x43);
//     Wire.endTransmission(false);
//     Wire.requestFrom(mpu_addr,2);
//     double GyX=Wire.read()<<8|Wire.read();
//     Wire.endTransmission(true);
//     return GyX;
// }

// double Acc_Y()
// {
//     Wire.beginTransmission(mpu_addr);
//     Wire.write(0x3D);
//     Wire.endTransmission(false);
//     Wire.requestFrom(mpu_addr,2);
//     double AccY=Wire.read()<<8|Wire.read();
//     Wire.endTransmission(true);
//     return AccY;  
// }

// double Acc_Z()
// {
//     Wire.beginTransmission(mpu_addr);
//     Wire.write(0x3F);
//     Wire.endTransmission(false);
//     Wire.requestFrom(mpu_addr,2);
//     double AccZ=Wire.read()<<8|Wire.read();
//     Wire.endTransmission(true);
//     return AccZ;  
// }