
#include "Arduino.h" //TODO: uncomment when using the arduino
#include "globals.h"

#define BLUE_PIN 3
#define YELLOW_PIN 5
#define GREEN_PIN 6

#define FSM_TICK_PERIOD_MS 2000
#define LED_SHOW_DURATION 2000
#define LED_FLASHING_ON_TIME 2000
#define LED_FLASHING_OFF_TIME 2000

typedef enum
{
  ON,
  OFF
} ledState;

class LedBoard {

    public:

    //Leds state machine global variables
    static bool flashLeds;
    static bool ledsIsOperational;

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
