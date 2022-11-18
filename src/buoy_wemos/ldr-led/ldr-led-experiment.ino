#define SWITCH D5
#define LEDPIN D4
#define SENSOR A0
#define INCREMENT 10

/**
 * Experiment: Observe changes in light received by LDR when different concentrations
 * of coffee is placed between the LDR and LED.
 */



volatile long last_pressed_time = 0;

int brightnessLevel = 0;
int brightnessDir = 0;
int sensorValue = 0; // variable to store the value coming from the sensor

/**
 * Function to cycle through the brightness of the LED so that we can find a sweet spot
 * to test for changes in light level.
 */
void cycle() {
  if (brightnessDir == 0) {
    brightnessLevel += INCREMENT;
    if (brightnessLevel >= 250) {
        brightnessDir = 1;
    }
  }
  else {
    brightnessLevel -= INCREMENT;
    if (brightnessLevel <= 0) {
        brightnessDir = 0;
    }
  }

  analogWrite(LEDPIN, brightnessLevel);
}

IRAM_ATTR void toggle() {

 long pressed = millis();

 if (pressed - last_pressed_time < 400) {
  return;
 }

 last_pressed_time = pressed;
 cycle();
}


void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH), toggle, RISING);
  analogWrite(LEDPIN, brightnessLevel);
}


void loop() {

  sensorValue = analogRead(SENSOR);
  Serial.printf("Light received %d, Brightness: %d \n", sensorValue, brightnessLevel);

  delay(50);
}