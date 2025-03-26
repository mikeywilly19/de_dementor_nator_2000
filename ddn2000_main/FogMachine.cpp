
#include "FogMachine.h"

//************************* FOG MACHINE **********************************

//Fog state machine:
//Desc: Starts with the fog machine being turned off and cold.
//      In response to the startBuildup and triggerFog variables
//      this state machine will start warming up and spew fog respectivley.

void FogMachine::setup() {
  //pinmodes
  pinMode(WARMUP_PIN, OUTPUT);
  pinMode(RELEASE_PIN, OUTPUT);
  
  //Fog machine
  startBuildup = false;
  releaseFog = false;
  canReleaseFog = false;

  setPinTurnOn(false);
  setPinRelease(false);
}

void FogMachine::setPinTurnOn(bool isTurnOn) {
  printf("turned on/off\n");
  //A7: release 
  //A6: warmup
  if (isTurnOn) {
    digitalWrite(WARMUP_PIN, LOW);
   } else {
    digitalWrite(WARMUP_PIN, HIGH);
   }
}

void FogMachine::setPinRelease(bool isRelease) {
  printf("released fog / stopped releasing fog\n");
  if (isRelease) {
    digitalWrite(RELEASE_PIN, LOW);
  } else {
    digitalWrite(RELEASE_PIN, HIGH);
  }
}

void debugPrintState(fogState state) {
  static fogState prevState = state;
  static bool first = true;
  if (first || prevState != state) {
    prevState = state;
    first = false;
    printf("state:%d\n", state);
  }
}

void FogMachine::tick() {

  //variables
  static fogState currFogState = FOG_COLD;
  static uint32_t elapsedFogTimeMS = 0;
  static uint32_t buildup_bound = FOG_BUILDUP_BOUND;
  
  //always keep track of how much time has passed (since last initialized to 0)
  elapsedFogTimeMS += FSM_TICK_PERIOD_MS;

  debugPrintState(currFogState);

  switch(currFogState) {
    case FOG_COLD:
      //transition
      if (startBuildup) {
        currFogState = FOG_BUILDUP;
        elapsedFogTimeMS = 0;
        canReleaseFog = false;
        setPinTurnOn(true);

        //buildup time is normal
        buildup_bound = FOG_BUILDUP_BOUND;
      }

      //action: stay in here until the fog machine is needed

      break;
    case FOG_BUILDUP:
      //transition
      if (!startBuildup) {
        currFogState = FOG_COLD;
        setPinTurnOn(false);
      } else if (releaseFog) {
        currFogState = FOG_RELEASE;
        releaseFog = false;
        canReleaseFog = false;
        elapsedFogTimeMS = 0;
        setPinRelease(true);
      }

      //action
      if (elapsedFogTimeMS >= buildup_bound) {
        canReleaseFog = true;
      }

      //when the buildup bound reaches the pin depress bound stop pressing release
      if (elapsedFogTimeMS >= FOG_PIN_DEPRESS_BOUND && elapsedFogTimeMS < buildup_bound) {
        setPinRelease(false);
      }

      break;
    case FOG_RELEASE:
      //transition
      if (!startBuildup) {
        currFogState = FOG_COLD;
        canReleaseFog = false;
        elapsedFogTimeMS = 0;
        setPinTurnOn(false);
        setPinRelease(false);
      } else if (elapsedFogTimeMS >= FOG_RELEASE_BOUND) {
        currFogState = FOG_BUILDUP;
        elapsedFogTimeMS = 0;
        canReleaseFog = false;
        setPinRelease(true);

        //TODO:since we just released the buildup time is smaller????
        buildup_bound = FOG_BUILDUP_BOUND_FROM_RELEASE;
      }

      //action
      //when the buildup bound reaches the pin depress bound stop pressing release
      if (elapsedFogTimeMS >= FOG_PIN_DEPRESS_BOUND && elapsedFogTimeMS < FOG_RELEASE_BOUND) {
        setPinRelease(false);
      }

      break;
    default:
      break;
  }
}

void FogMachine::start() {
  startBuildup = true;
}

void FogMachine::release(bool isRelease) {
  if (isRelease && canReleaseFog) {
    releaseFog = true;
  }
  if (!isRelease) {
    releaseFog = false;
  }
  //else there is no notification that there can't be a release for fog
}

bool FogMachine::canRelease() {
  return canReleaseFog;
}

void FogMachine::stop() {
  startBuildup = false;
  tick(); //tick to update the state machine and stop processes
}
