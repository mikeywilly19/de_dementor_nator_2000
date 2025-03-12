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
#include "Speaker.h"

Speaker speaker = Speaker();

#define FSM_TICK_FREQ_HZ 100.0
#define FSM_TICK_PERIOD_MS 1000.0 / FSM_TICK_FREQ_HZ

// Pins
#define FOG_TRIGGER
#define SOUND_TRIGGER
#define BLUE_PIN 5
#define YELLOW_PIN 4
#define GREEN_PIN 3
#define RED_PIN 7
#define PIR_SENSOR 2
#define MANUAL_TRIGGER A2

// LED defines (ms)
#define TIME_FLASHING 3000
#define FLASH_DURATION 100

#define WARMUP_TIME 1000 // 10s

typedef enum
{
  INIT,
  WARMUP,
  WAIT,
  STARTFOG,
  STARTSHOW,
  SHUTDOWN
} fsmState;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  speaker.setup();
  // set inputs
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT_PULLUP);

  // set outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  // Simple scheduler to call tick function with constant frequency
  int startTime = millis();
  fsmTick();
  speaker.tick();
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
    // wait for warm up time
    static uint32_t elapsedTimeMS = 0;
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    if (elapsedTimeMS >= WARMUP_TIME) state = WAIT;
    break;
  case WAIT:
    // no actions
    // transitions
    if (digitalRead(PIR_SENSOR) || !digitalRead(MANUAL_TRIGGER)) state = STARTFOG;
    break;
  case STARTFOG:
    // trigger fog
    
    // transition
    state = STARTSHOW;
    break;
  case STARTSHOW:
    // actions
    digitalWrite(LED_BUILTIN, HIGH);
    // start sound
    flashLEDs(TIME_FLASHING);
    speaker.start();

    // transitions
    state = SHUTDOWN;
    break;
  case SHUTDOWN:
    digitalWrite(LED_BUILTIN, LOW);
    // stop everything
    
    // transitions
    state = WARMUP;
    break;
  default:
    break;
  }
}

// repeatedly flash each LED color individually for a given amount of time
void flashLEDs(uint32_t timeOut) {
  uint32_t time = millis();
  static bool ledsOn = false;

  while ((time + timeOut) > millis()) {
    if (ledsOn) {
      // write low
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
    }
    else {
      // write high
      digitalWrite(BLUE_PIN, HIGH);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(RED_PIN, HIGH);
    }
    ledsOn = !ledsOn;
    delay(FLASH_DURATION);
  }

  // turn off
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(RED_PIN, LOW);

}

//************************ Audio ***************************************//

//void tickAudio() {
//}
//}
