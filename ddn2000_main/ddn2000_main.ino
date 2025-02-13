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

#define FSM_TICK_FREQ_HZ 100.0
#define FSM_TICK_PERIOD_MS 1000.0 / FSM_TICK_FREQ_HZ

typedef enum
{
  INIT,
  WARMUP,
  WAIT,
  FOG,
  LED,
  SOUND,
  SHUTDOWN
} fsmState;

void setup()
{
  // Init for all subsystems
}

void loop()
{
  // Simple scheduler to call tick function with constant frequency
  int startTime = millis();
  fsmTick();
  while ((startTime + FSM_TICK_PERIOD_MS) > millis())
  {
  }
}

void fsmTick()
{
  static fsmState state = INIT;

  switch (state)
  {
  case INIT:
    state = WARMUP;
    break;
  case WARMUP:
    break;
  case WAIT:
    break;
  case FOG:
    break;
  case LED:
    break;
  case SOUND:
    break;
  case SHUTDOWN:
    break;
  default:
    break;
  }
}
