#ifndef FOG_MACHINE_H
#define FOG_MACHINE_H

 #include "Arduino.h" //TODO: uncomment when using the arduino
//#include <iostream>
#include "globals.h"

#define FOG_BUILDUP_BOUND 180000 // Warmup Time - 3 minutes
#define FOG_BUILDUP_BOUND_FROM_RELEASE 60000 // Warmup time after Trigger - 1 minute
#define FOG_RELEASE_BOUND 2000 // Time to hold release button on
#define FOG_PIN_DEPRESS_BOUND 100

#define WARMUP_PIN 7
#define RELEASE_PIN 8

typedef enum
{
  FOG_COLD,
  FOG_BUILDUP,
  FOG_RELEASE
} fogState;

class FogMachine {

    public:

    //Fog state machine global variables
    bool startBuildup;
    bool releaseFog;
    bool canReleaseFog;

    void setup();
    void tick();
    void start();
    void stop();
    void release();
    bool canRelease();

    private:
    void setPinTurnOn(bool isTurnOn);
    void setPinRelease(bool isRelease);

};

#endif
