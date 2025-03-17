
#include "LedBoard.h"

//***************************** LEDS *************************************

//LED state machine:
//Desc: Initially is turned off until the variable flashLeds is set true.
//      Once flashLeds is set true, the leds flash until flashLeds is set to false.

void LedBoard::setup() {
  printf("leds: setup reached\n");
  //output pins
  // pinMode(GREEN_PIN, OUTPUT);
  // pinMode(BLUE_PIN, OUTPUT);
  // pinMode(YELLOW_PIN, OUTPUT);

  //LED setup
  flashLeds = false;
  ledsIsOperational = false;
}

void LedBoard::tick() {
  static int elapsedLTimeMS = 0;
  elapsedLTimeMS += FSM_TICK_PERIOD_MS;

  if (ledsIsOperational) {
    //transition
    if (elapsedLTimeMS >= LED_SHOW_DURATION) {
      ledsIsOperational = false;
      elapsedLTimeMS = 0;
      flash(false);
    }

    //action
    flashTick();
  } else {
    //no transition out

    //action: do nothing
  }
}

void LedBoard::writePins(bool isHigh) {
  // if (isHigh) {
  //   digitalWrite(BLUE_PIN, HIGH);
  //   digitalWrite(YELLOW_PIN, HIGH);
  //   digitalWrite(GREEN_PIN, HIGH);
  // } else {
  //   digitalWrite(BLUE_PIN, LOW);
  //   digitalWrite(YELLOW_PIN, LOW);
  //   digitalWrite(GREEN_PIN, LOW);
  // }

  //debug: print what is written to the pins instead
  // printf("leds: (high is 1 low is 0): %d\n", isHigh);
}

void debugPrintFlashState(ledState state) {
  static ledState prevState = state;
  static bool first = true;
  if (first || prevState != state) {
    prevState = state;
    first = false;
    printf("state:%d\n", state);
  }
}
  
void LedBoard::flashTick() {
  static ledState lState = OFF;
  static int elapsedLedTimeMS = 0;
  elapsedLedTimeMS += FSM_TICK_PERIOD_MS;
  debugPrintFlashState(lState);

  switch(lState) {
    case OFF:
      if (elapsedLedTimeMS >= LED_FLASHING_OFF_TIME && flashLeds) {
        lState = ON;
        elapsedLedTimeMS = 0;
        // write high on transition
        writePins(true);
      }
      break;
    case ON:
      // transitions
      if (elapsedLedTimeMS >= LED_FLASHING_ON_TIME || !flashLeds) {
        lState = OFF;
        elapsedLedTimeMS = 0;
        // write low
        writePins(false);
      }
      
      break;
    default:
      break;
  }
}
  
void LedBoard::flash(bool turnOn) {
  flashLeds = turnOn;
  if (flashLeds == false) {
    //Tick the state machine once to update the lights
    flashTick();
  }
}

bool LedBoard::isFlashing() {
  return flashLeds;
}

void LedBoard::start() {
  printf("called start\n");
  //the start of a controller for the leds (because leds should be seperate from the scheduler)
  ledsIsOperational = true;
  flash(true);
}

//*************************************************************************
