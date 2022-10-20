#define PHOTO_PIN A0

void initialize_photores() {
    pinMode(PHOTO_PIN, INPUT);
}

float read_photores() {
    // returns the current brightness as a relative value from 0.0 - 1.0
    int v = analogRead(PHOTO_PIN);
    return v / 1024.0;
}

/*
    example

void setup() {
    Serial.begin(9600);
    initialize_photores();
}

void loop() {
    float v = read_photores();
    Serial.print("brightness:\t\t");
    Serial.println(v);
}
*/