#define BLUE_PIN 5
#define YELLOW_PIN 4
#define GREEN_PIN 3

#define PIR_SENSOR 2

#define TIME_FLASHING 3000

uint8_t LEDS[3] = {BLUE_PIN, YELLOW_PIN, GREEN_PIN};

void setup() {
  // set inputs
  pinMode(PIR_SENSOR, INPUT);

  // set outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // flash LEDs with motion
  if (digitalRead(PIR_SENSOR)) {
    Serial.println("MOTION!!!");
    flashLEDs(TIME_FLASHING);
  }
  else {
    Serial.println("nothing");
  }
}

// repeatedly flash each LED color individually a certain number of cycles
void flashLEDs(uint32_t timeOut) {
  uint32_t time = millis();

  while ((time + timeOut) > millis()) {
    for (uint8_t k = 0; k < 3; k++) {
      digitalWrite(LEDS[k], HIGH);
      delay(DELAY);
      digitalWrite(LEDS[k], LOW);
      delay(DELAY);
    }
  }
}
