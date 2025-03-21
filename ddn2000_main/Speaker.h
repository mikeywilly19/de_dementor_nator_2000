#ifndef CLASS_SPEAKER
#define CLASS_SPEAKER

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#define FSM_TICK_PERIOD_MS 100

#define FPSerial softSerial

class Speaker {

  public:

  SoftwareSerial softSerial(10, 11);
  DFRobotDFPlayerMini myDFPlayer;
  bool isStarted = false;
  
  int state = 0;
  uint32_t count = 0;
  bool first = true;

  void setup();
  void start();
  void stop();
  void tick();

  bool getIsPlaying();

};

#endif
