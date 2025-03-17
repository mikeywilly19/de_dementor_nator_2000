#ifndef LED_BOARD_H
#define LED_BOARD_H

 #include "Arduino.h" //TODO: uncomment when using the arduino
#include "globals.h"
//#include <iostream>

#define BLUE_PIN 3
#define YELLOW_PIN 5
#define GREEN_PIN 6

//all in milli seconds
#define LED_SHOW_DURATION 2000
#define LED_FLASHING_ON_TIME 500
#define LED_FLASHING_OFF_TIME 500

typedef enum
{
  ON,
  OFF
} ledState;

class LedBoard {

    public:
    LedBoard() {}

    //Leds state machine global variables
    bool flashLeds;
    bool ledsIsOperational;

    void setup();
    void tick();
    bool isFlashing();
    void start();
    void stop();

    private:

    void flashTick();
    void flash(bool turnOn);
    void writePins(bool isHigh);
    

};

#endif
