#include <MPU6500_WE.h>
#include <Wire.h>
#define MPU6500_ADDR 0x68

MPU6500_WE mpu = MPU6500_WE(MPU6500_ADDR);

void initialize_mpu() {
	// calibrates the MPU and initializes the I2C connection
	// based on: https://github.com/wollewald/MPU9250_WE/blob/main/examples/MPU6500_all_data/MPU6500_all_data.ino

	if(!mpu.init()){
		Serial.println("no response from MPU (yet)");
	}else{
		Serial.println("MPU is connected");
	}

	/* The slope of the curve of acceleration vs measured values fits quite well to the theoretical 
	* values, e.g. 16384 units/g in the +/- 2g range. But the starting point, if you position the 
	* MPU6500 flat, is not necessarily 0g/0g/1g for x/y/z. The autoOffset function measures offset 
	* values. It assumes your MPU6500 is positioned flat with its x,y-plane. The more you deviate 
	* from this, the less accurate will be your results.
	* The function also measures the offset of the gyroscope data. The gyroscope offset does not   
	* depend on the positioning.
	* This function needs to be called at the beginning since it can overwrite your settings!
	*/
	Serial.println("Position the device flat and don't move it - calibrating...");
	delay(1000);
	mpu.autoOffsets();
	Serial.println("Done!");

	/*  This is a more accurate method for calibration. You have to determine the minimum and maximum 
	*  raw acceleration values of the axes determined in the range +/- 2 g. 
	*  You call the function as follows: setAccOffsets(xMin,xMax,yMin,yMax,zMin,zMax);
	*  Use either autoOffset or setAccOffsets, not both.
	*/
	//mpu.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);

	/*  The gyroscope data is not zero, even if you don't move the MPU6500. 
	*  To start at zero, you can apply offset values. These are the gyroscope raw values you obtain
	*  using the +/- 250 degrees/s range. 
	*  Use either autoOffset or setGyrOffsets, not both.
	*/
	//mpu.setGyrOffsets(45.0, 145.0, -105.0);

	/*  You can enable or disable the digital low pass filter (DLPF). If you disable the DLPF, you 
	*  need to select the bandwdith, which can be either 8800 or 3600 Hz. 8800 Hz has a shorter delay,
	*  but higher noise level. If DLPF is disabled, the output rate is 32 kHz.
	*  MPU6500_BW_WO_DLPF_3600 
	*  MPU6500_BW_WO_DLPF_8800
	*/
	mpu.enableGyrDLPF();
	//mpu.disableGyrDLPF(MPU6500_BW_WO_DLPF_8800); // bandwdith without DLPF

	/*  Digital Low Pass Filter for the gyroscope must be enabled to choose the level. 
	*  MPU6500_DPLF_0, MPU6500_DPLF_2, ...... MPU6500_DPLF_7 
	*  
	*  DLPF    Bandwidth [Hz]   Delay [ms]   Output Rate [kHz]
	*    0         250            0.97             8
	*    1         184            2.9              1
	*    2          92            3.9              1
	*    3          41            5.9              1
	*    4          20            9.9              1
	*    5          10           17.85             1
	*    6           5           33.48             1
	*    7        3600            0.17             8
	*    
	*    You achieve lowest noise using level 6  
	*/
	mpu.setGyrDLPF(MPU6500_DLPF_6);

	/*  Sample rate divider divides the output rate of the gyroscope and accelerometer.
	*  Sample rate = Internal sample rate / (1 + divider) 
	*  It can only be applied if the corresponding DLPF is enabled and 0<DLPF<7!
	*  Divider is a number 0...255
	*/
	mpu.setSampleRateDivider(5);

	/*  MPU6500_GYRO_RANGE_250       250 degrees per second (default)
	*  MPU6500_GYRO_RANGE_500       500 degrees per second
	*  MPU6500_GYRO_RANGE_1000     1000 degrees per second
	*  MPU6500_GYRO_RANGE_2000     2000 degrees per second
	*/
	mpu.setGyrRange(MPU6500_GYRO_RANGE_250);

	/*  MPU6500_ACC_RANGE_2G      2 g   (default)
	*  MPU6500_ACC_RANGE_4G      4 g
	*  MPU6500_ACC_RANGE_8G      8 g   
	*  MPU6500_ACC_RANGE_16G    16 g
	*/
	mpu.setAccRange(MPU6500_ACC_RANGE_2G);

	/*  Enable/disable the digital low pass filter for the accelerometer 
	*  If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
	*/
	mpu.enableAccDLPF(true);

	/*  Digital low pass filter (DLPF) for the accelerometer, if enabled 
	*  MPU6500_DPLF_0, MPU6500_DPLF_2, ...... MPU6500_DPLF_7 
	*   DLPF     Bandwidth [Hz]      Delay [ms]    Output rate [kHz]
	*     0           460               1.94           1
	*     1           184               5.80           1
	*     2            92               7.80           1
	*     3            41              11.80           1
	*     4            20              19.80           1
	*     5            10              35.70           1
	*     6             5              66.96           1
	*     7           460               1.94           1
	*/
	mpu.setAccDLPF(MPU6500_DLPF_6);

	/* You can enable or disable the axes for gyroscope and/or accelerometer measurements.
	* By default all axes are enabled. Parameters are:  
	* MPU6500_ENABLE_XYZ  //all axes are enabled (default)
	* MPU6500_ENABLE_XY0  // X, Y enabled, Z disabled
	* MPU6500_ENABLE_X0Z   
	* MPU6500_ENABLE_X00
	* MPU6500_ENABLE_0YZ
	* MPU6500_ENABLE_0Y0
	* MPU6500_ENABLE_00Z
	* MPU6500_ENABLE_000  // all axes disabled
	*/
	//mpu.enableAccAxes(MPU6500_ENABLE_XYZ);
	//mpu.enableGyrAxes(MPU6500_ENABLE_XYZ);
}

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