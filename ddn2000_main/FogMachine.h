#ifndef FOG_MACHINE_H
#define FOG_MACHINE_H

 #include "Arduino.h" //TODO: uncomment when using the arduino
//#include <iostream>
#include "globals.h"

#define FOG_BUILDUP_BOUND 2000 //2s
#define FOG_BUILDUP_BOUND_FROM_RELEASE 1000 //1s
#define FOG_RELEASE_BOUND 2000 //2s

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
