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
#include "LedBoard.h"
#include "FogMachine.h"
#include "globals.h"

Speaker speaker = Speaker();
LedBoard ledBoard = LedBoard();
FogMachine fogMachine = FogMachine();

// Pins
#define SOUND_TRIGGER 
#define PIR_SENSOR A5
#define MANUAL_TRIGGER A3

// ****** State machinees:
// Scheduler
#define SCHED_WARMUP_TIME 1000 // 1s
#define SCHED_FOG_BUILDUP_TIME 1000 //1s
#define SCHED_SHOW_TIME_BOUND 6000 //1s (be greater than 201)
#define SCHED_FOG_RELEASE_TIME 1000 //1s

// State LEDs
#define WARMUP_LED A2
#define WAITING_LED A1
#define STARTFOG_LED A0
//#define SHUTDOWN_LED

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
  // Serial
  Serial.begin(9600);
  delay(2000);
  Serial.println("wstarting");

  // set inputs
  // the sensor
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT_PULLUP);

  //debug user notification light
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // set outputs
  pinMode(WARMUP_LED, OUTPUT);
  pinMode(WAITING_LED, OUTPUT);
  pinMode(STARTFOG_LED, OUTPUT);
  //pinMode(SHUTDOWN_LED, OUTPUT);
  fogMachine.setup();
  ledBoard.setup();
  speaker.setup();
}

void loop()
{
  // Simple scheduler to call tick function with constant frequency
  uint32_t startTime = millis();
  fsmTick();
  fogMachine.tick();
  ledBoard.tick();
  speaker.tick();
  while ((startTime + FSM_TICK_PERIOD_MS) > millis())
  {
//    Serial.print("Current Time: ");
//    Serial.print(millis());
//    Serial.print("Time to go: ");
//    Serial.println(startTime + FSM_TICK_PERIOD_MS);
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
  static bool isMachineOn = false;
  static uint32_t elapsedTimeMS = 0;

  //TODO: update isMachineOn with the value of a switch (not a fixed value)
  isMachineOn = true;



  switch (state)
  {
  case INIT:
    if (isMachineOn) {
      state = WARMUP;
      fogMachine.start();
    } else {
      state = SHUTDOWN;
    }
    
    break;
  case WARMUP:
    digitalWrite(STARTFOG_LED, LOW); // state light off
    digitalWrite(WARMUP_LED, HIGH); // state light on
    
    //transition
    if (!isMachineOn) {
      state = SHUTDOWN;
      fogMachine.stop();
    } else if (fogMachine.canRelease()) {
      state = WAIT;
      elapsedTimeMS = 0;

      //User notification that we can release fog
      digitalWrite(LED_BUILTIN, HIGH);
    }

    //action: wait for the fog machine to warm up

    break;
  case WAIT:
    digitalWrite(WARMUP_LED, LOW); // state light off
    digitalWrite(WAITING_LED, HIGH); // state light on
  
    // transitions
    if (!isMachineOn) {
      state = SHUTDOWN;
      fogMachine.stop();
    } else if (digitalRead(PIR_SENSOR) || digitalRead(MANUAL_TRIGGER)) {
      if(digitalRead(PIR_SENSOR)) {
        Serial.println("Motion Sensor");
      }
      //Someone walked by, start to trigger show
      state = STARTFOG;
      fogMachine.release();
      elapsedTimeMS = 0;

      //Stop giving User notification as we release fog
      digitalWrite(LED_BUILTIN, LOW);
    }

    // no actions

    break;
  case STARTFOG:
    digitalWrite(WAITING_LED, LOW); // state light off
    digitalWrite(STARTFOG_LED, HIGH); // state light on
    //transition
    if (!isMachineOn) {
      state = SHUTDOWN;
      fogMachine.stop();
    } else if (elapsedTimeMS >= SCHED_FOG_RELEASE_TIME) {
      state = STARTSHOW;
      elapsedTimeMS = 0;

      //start speakers and leds
      speaker.start();
      ledBoard.start();
    }

    //action: wait for fog to release + amount of time for delaying the show
    elapsedTimeMS += FSM_TICK_PERIOD_MS;

    break;
  case STARTSHOW:
    //In this state the fog should have already released and the
    //  speaker and leds have been notified to begin their show
    
    //transition
    if (!isMachineOn) {
      //forcefully end all speaker, fog and led show stuff
      state = SHUTDOWN;
      fogMachine.stop();
      speaker.stop();
      ledBoard.stop();
    } else if ( !speaker.isPlaying() && !ledBoard.isFlashing()) {
      //finished show, go back to warmup for next show (since the machine is still on)
      state = WARMUP;
    }
    
    //action: wait for all state machines to do their thing
    elapsedTimeMS += FSM_TICK_PERIOD_MS;
    
    break;
  case SHUTDOWN:
    // stop everything on entering this state 
    // (this is also initially where everything is off)
    
    //transition
    if (isMachineOn) {
      //start warming up the fog machine for operation
      state = WARMUP;
      fogMachine.start();
    }

    //action: machine is off, do nothing

    break;
  default:
    break;
  }

  Serial.println(state);
}
