# nus-cs3237-iot-buoys

## Project Proposal

**Team Members**:  

- Pinkl Constantin Maxime (A0260806H),  
- Teo Chuan Kai (A0174217H),  
- Thomm Leon Felix (A0262962X), 
- Wong Chee Hong (A0217558W), 
- Lai Yu Heem (A0201317X)  

**Project/Team Title**: Underwater Weather Forecast

**Introduction / Problem Statement**:

Weather forecasts are generally used to predict weather conditions for a wide range of applications, such as outdoor activities. For marine sports such as surfing or scuba diving, weather forecasts are useful but may not be sufficient in predicting the current conditions of the sea. Weather forecasts do not provide localized information about how turbulent or visible the sea may be. Such data collected may also be applicable in the long-term for climate scientists that are studying patterns between weather conditions and sea conditions over time. 

**Proposed IoT Solution**:

We develop buoys which collect information about the current water conditions such as water turbulence (waves), water visibility and water temperature. A single buoy can be generalized to multiple buoys to scale. Each buoy may intermittently communicate with neighboring buoys to wake up sleeping buoys for better energy management when it detects some change of interest (certain threshold).

or the project we will be demonstrating the solution with two buoys. 
 

**Sensors/Actuators/Hardware Used**: 

Sensors/devices used per buoy:

- Phone (Accelerometer, Camera, Flashlight, GPS, Internet) 
- A portable power bank acting as a power source 
- WeMOS D1 
- Temperature sensor 
- Photoresistor 
- Accelerometer (waves) 
- IR sensor 

Physical items: 

- A large tub to hold water for simulation purposes. 
- Waterproof container for buoy 

**Machine Learning Models**: 
- Classifier for defining underwater visibility based on light intensity 
- Classifier for defining water turbulence based on accelerometer data 
- Classifier to predict future visibility underwater using all data provided by the Buoy. 

**System Architecture**: 

Phone app combined with WeMOS controller and sensors that collects data and sends it to the cloud. 

Multiple buoys collect data that they send to the cloud, where it is processed. 

Predicted future data and real time data can be accessed through a web interface.  

**Other Cool Features**:  

- Building a real-time web interface 
- Creating a water-proof enclosure for our sensors and phones 

**Possible Limitations or Challenges**:

- Demonstrating the system may have to be attempted at a reduced scale, such as using a container to replicate the ocean. 
- Existing data for parameters such as wave movements, water temperature of a certain location may be difficult to access, making collection of data to train a ML model difficult. 
- In order to utilise the accelerometers from our phones, we may have to develop applications that communicate with the sensors within the phone. 
- Waterproofing our enclosures.

**Timeline**:  

Until 15 October (Check In 1): [two weeks] 

    - Figuring out all the sensors we want to use (underwater temperature, IR, accelerometer, light detector, etc.). 
    - Writing some functions for using the sensors in our setup. 

Until 29 October (Check in 2): [two weeks] 

    - Get all equipment to build the buoy. 
    - Assemble the buoys. 
    - Start collecting data. 

Until 7 November (Presentation Week): [one week] 

    - Collect data, train models, make predictions. 
    - Finish project 

Until 20 November (Submission): [two weeks] 

    - Submit Report 
