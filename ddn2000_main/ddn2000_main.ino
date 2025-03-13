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

#define FSM_TICK_PERIOD_MS 100

// Pins
#define FOG_TRIGGER
#define FOG_WARMUP
#define SOUND_TRIGGER
#define BLUE_PIN 3
#define YELLOW_PIN 5
#define GREEN_PIN 6
#define PIR_SENSOR A5
#define MANUAL_TRIGGER A3
#define VOLUME 20


// LED defines (ms)
#define TIME_FLASHING 3000
#define FLASH_DURATION 200

#define WARMUP_TIME 1000 // 1s
#define FOG_BUILDUP_TIME 1000 //1s
#define SHOW_TIME_BOUND 10000 //1s (be greater than 201)

typedef enum
{
  INIT,
  WARMUP,
  WAIT,
  STARTFOG,
  STARTSHOW,
  SHUTDOWN
} fsmState;

typedef enum
{
  ON,
  OFF
} ledState;

static bool startLeds;

void setup()
{
  // Serial
  Serial.begin(9600);

  // set inputs
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT_PULLUP);

  // set outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //speaker setup
  speaker.setup();
  speaker.setVolume(VOLUME);

  //LED setup
  startLeds = false;
}

void loop()
{
  // Simple scheduler to call tick function with constant frequency
  int startTime = millis();
  fsmTick();
  speaker.tick();
  ledTick();
  while ((startTime + FSM_TICK_PERIOD_MS) > millis())
  {
  }
}

void fsmTick()
{
  static fsmState state = INIT;
  static uint32_t elapsedTimeMS = 0;
  Serial.print(state);

  switch (state)
  {
  case INIT:
    state = WARMUP;
    break;
  case WARMUP:
    //transition
    if (elapsedTimeMS >= WARMUP_TIME) {
      state = WAIT;
      digitalWrite(LED_BUILTIN, HIGH);
      elapsedTimeMS = 0;
    }

    //action
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    break;
  case WAIT:
    // no actions
    // transitions
    if (digitalRead(PIR_SENSOR) || digitalRead(MANUAL_TRIGGER)) {
      state = STARTFOG;
      //TODO:trigger fog
//      digitalWrite(FOG_TRIGGER, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
    }
    break;
  case STARTFOG:
    //action
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    
    //transition
    if (elapsedTimeMS >= FOG_BUILDUP_TIME) {
      state = STARTSHOW;
      elapsedTimeMS = 0;

      //start any state machines (that are asynchronous with timing of show state)
      speaker.start();
      startLeds = true;
    }
    
    break;
  case STARTSHOW:
    //action
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    //TODO: fog machine spit out fog
    
    
    //transition
    if (elapsedTimeMS >= SHOW_TIME_BOUND) {
      state = SHUTDOWN;
      elapsedTimeMS = 0;
      startLeds = false;
    }
    
    break;
  case SHUTDOWN:
    // stop everything
    
    // transitions
    state = WARMUP;
    break;
  default:
    break;
  }
}

// repeatedly flash each LED color individually for a given amount of time
void ledTick() {
  static ledState lState = OFF;
  static uint32_t elapsedLedTimeMS = 0;
  elapsedLedTimeMS += FSM_TICK_PERIOD_MS;

  switch(lState) {
    case OFF:
      if (elapsedLedTimeMS >= FLASH_DURATION && startLeds) {
        lState = ON;
        elapsedLedTimeMS = 0;
        // write high on transition
        digitalWrite(BLUE_PIN, HIGH);
        digitalWrite(YELLOW_PIN, HIGH);
        digitalWrite(GREEN_PIN, HIGH);
      }
      break;
    case ON:
      // transitions
      if (elapsedLedTimeMS >= FLASH_DURATION) {
        lState = OFF;
        elapsedLedTimeMS = 0;
        // write low
        digitalWrite(BLUE_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
      }
      
      break;
    default:
      break;
  }
}
