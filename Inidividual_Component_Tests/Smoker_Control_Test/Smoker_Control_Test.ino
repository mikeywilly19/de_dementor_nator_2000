// Smoke Maker Test code


#define SMOKER_POWER_PIN 8
#define BUTTON_PIN A3
#define LED_PIN 13

void setup() {
  // put your setup code here, to run once:
  pinMode(SMOKER_POWER_PIN, OUTPUT);
  pinMode(SMOKER_TRIGGER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  static bool isOn = false;
  if(analogRead(BUTTON_PIN) > 600) {
    digitalWrite(SMOKER_POWER_PIN, isOn?LOW:HIGH);
    digitalWrite(SMOKER_TRIGGER_PIN, isOn?LOW:HIGH);
    digitalWrite(LED_PIN, isOn?LOW:HIGH);
    isOn = !isOn;
    Serial.println("Button push detected");
  } else {
    Serial.println("No Button detected");
  }
  delay(1000);
}
