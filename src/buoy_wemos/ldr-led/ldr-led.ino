 
int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = D4; 
int brightnessLevel = 0;

int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}
void loop() {

  analogWrite(D4, brightnessLevel);
  delay(20);
  sensorValue = analogRead(sensorPin);


  if (sensorValue == 1024 || brightnessLevel == 255) { //maxed value
    Serial.println(brightnessLevel);
  } else if (sensorValue < 1024) {
    Serial.printf("Light received %d, Brightness: %d \n", sensorValue, brightnessLevel);
    brightnessLevel += 1;
  }
}