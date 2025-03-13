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

// ****** State machinees:
// Scheduler
#define SCHED_WARMUP_TIME 1000 // 1s
#define SCHED_FOG_BUILDUP_TIME 1000 //1s
#define SCHED_SHOW_TIME_BOUND 10000 //1s (be greater than 201)

//Fog defines
#define FOG_BUILDUP_BOUND 2000 //2s
#define FOG_BUILDUP_BOUND_FROM_RELEASE 1000 //1s
#define FOG_RELEASE_BOUND 2000 //2s

// LED defines (ms)
#define TIME_FLASHING 3000
#define FLASH_DURATION 200

//Speaker defines are found in Speaker.h

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

typedef enum
{
  FOG_COLD,
  FOG_BUILDUP,
  FOG_RELEASE
} fogState;

static bool flashLeds;
static bool startBuildup;
static bool releaseFog;
static bool canReleaseFog;

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

  //Fog machine
  startBuildup = false;
  releaseFog = false;
  canReleaseFog = false;

  //speaker setup
  speaker.setup();
  speaker.setVolume(VOLUME);

  //LED setup
  flashLeds = false;
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

//***************************** Scheduler ****************************************

// This defines the scheduler state machine which controls all other 
// state machines involved in the project including leds, fog and sound.
// (these other state machines are implemented in their respective tick functions)
// This state machine is ticked at a rate of FSM_TICK_PERIOD_MS
void fsmTick()
{
  static fsmState state = INIT;
  static uint32_t elapsedTimeMS = 0;

  switch (state)
  {
  case INIT:
    state = WARMUP;
    break;
  case WARMUP:
    //transition
    if (elapsedTimeMS >= SCHED_WARMUP_TIME) {
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
    if (elapsedTimeMS >= SCHED_FOG_BUILDUP_TIME) {
      state = STARTSHOW;
      elapsedTimeMS = 0;

      //start any state machines (that are asynchronous with timing of show state)
      speaker.start();
      ledsFlash(true);
    }
    
    break;
  case STARTSHOW:
    //action
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    //TODO: fog machine spit out fog
    
    
    //transition
    if (elapsedTimeMS >= SCHED_SHOW_TIME_BOUND) {
      state = SHUTDOWN;
      elapsedTimeMS = 0;
      ledsFlash(false);
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

//************************* FOG MACHINE **********************************

//Fog state machine:
//Desc: Starts with the fog machine being turned off and cold.
//      In response to the startBuildup and triggerFog variables
//      this state machine will start warming up and spew fog respectivley.
void fogTick() {

  //variables
  static fogState currFogState = FOG_COLD;
  static uint32_t elapsedFogTimeMS = 0;
  static uint32_t buildup_bound = FOG_BUILDUP_BOUND;
  
  //always keep track of how much time has passed (since last initialized to 0)
  elapsedFogTimeMS += FSM_TICK_PERIOD_MS;

  switch(currFogState) {
    case FOG_COLD:
      //transition
      if (startBuildup) {
        currFogState = FOG_BUILDUP;
        elapsedFogTimeMS = 0;
        canReleaseFog = false;

        //buildup time is normal
        buildup_bound = FOG_BUILDUP_BOUND;
      }

      //action: stay in here until the fog machine is needed

      break;
    case FOG_BUILDUP:
      //transition
      if (!startBuildup) {
        currFogState = FOG_COLD;
      } else if (releaseFog) {
        currFogState = FOG_RELEASE;
        elapsedFogTimeMS = 0;
      }

      //action
      if (elapsedFogTimeMS >= buildup_bound) {
        canReleaseFog = true;
      }
      //TODO:turn on the fog machine warm up system (may need to put in transition)

      break;
    case FOG_RELEASE:
      //transition
      if (!startBuildup) {
        currFogState = FOG_COLD;
      } else if (elapsedFogTimeMS >= FOG_RELEASE_BOUND) {
        currFogState = FOG_BUILDUP;
        elapsedFogTimeMS = 0;
        canReleaseFog = false;

        //TODO:since we just released the buildup time is smaller????
        buildup_bound = FOG_BUILDUP_BOUND_FROM_RELEASE;
      }

      //TODO:action (for some duration release the fog (may need to put in transition))

      break;
    default:
      break;
  }
}

void fogmachineTurnOn() {
  startBuildup = true;
}

void fogmachineReleaseFog() {
  if (canReleaseFog) {
    releaseFog = true;
  }
  //else there is no notification that there can't be a release for fog
}

void fogmachineTurnOff() {
  startBuildup = false;
}

//***************************** LEDS *************************************

//LED state machine:
//Desc: Initially is turned off until the variable flashLeds is set true.
//      Once flashLeds is set true, the leds flash until flashLeds is set to false.
void ledTick() {
  static ledState lState = OFF;
  static uint32_t elapsedLedTimeMS = 0;
  elapsedLedTimeMS += FSM_TICK_PERIOD_MS;

  switch(lState) {
    case OFF:
      if (elapsedLedTimeMS >= FLASH_DURATION && flashLeds) {
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
      if (elapsedLedTimeMS >= FLASH_DURATION || !flashLeds) {
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

void ledsFlash(bool turnOn) {
  flashLeds = turnOn;
  if (flashLeds == false) {
    //Tick the state machine once to update the lights
    ledTick();
  }
}

//*************************************************************************
