/**************************************
 * De-Dementor-nator 2000 main program
 *
 * Authors -  Michael Williams
 *            Caden Brinkman
 *            Joshua Gulbransen
 *            Blake Olenik
 *            Weston Willardson
 *
 * Class -    EcEn 446
 *            Winter 2025
 *            Steven Schultz
 **************************************/

#define FSM_TICK_FREQ_HZ 100.0
#define FSM_TICK_PERIOD_MS 1000.0 / FSM_TICK_FREQ_HZ

// Pins
#define BLUE_PIN 5
#define YELLOW_PIN 4
#define GREEN_PIN 3
#define PIR_SENSOR 2
#define MANUAL_TRIGGER A2

// LED defines (ms)
#define TIME_FLASHING 3000
#define FLASH_DURATION 100

typedef enum
{
  INIT,
  WARMUP,
  WAIT,
  FOG,
  LED,
  SOUND,
  SHUTDOWN
} fsmState;

void setup()
{
  // set inputs
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT_PULLUP);

  // set outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
}

void loop()
{
  // Simple scheduler to call tick function with constant frequency
  int startTime = millis();
  fsmTick();
  while ((startTime + FSM_TICK_PERIOD_MS) > millis())
  {
  }
}

void fsmTick()
{
  static fsmState state = INIT;

  switch (state)
  {
  case INIT:
    state = WARMUP;
    break;
  case WARMUP:
    break;
  case WAIT:
    break;
  case FOG:
    break;
  case LED:
    // transitions
    
    // actions
    if (digitalRead(PIR_SENSOR) || !digitalRead(MANUAL_TRIGGER)) flashLEDs(TIME_FLASHING); // flash LEDs when triggered
    
    break;
  case SOUND:
    break;
  case SHUTDOWN:
    break;
  default:
    break;
  }
}

// repeatedly flash each LED color individually for a given amount of time
void flashLEDs(uint32_t timeOut) {
  uint32_t time = millis();

  while ((time + timeOut) > millis()) {
    for (uint8_t k = 0; k < 3; k++) {
      digitalWrite(LEDS[k], HIGH);
      delay(FLASH_DURATION);
      digitalWrite(LEDS[k], LOW);
      delay(FLASH_DURATION);
    }
  }
}
