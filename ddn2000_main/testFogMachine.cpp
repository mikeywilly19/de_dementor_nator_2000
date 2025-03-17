//#include <iostream>
//#include <unistd.h>
//#include "FogMachine.h"
//#include "globals.h"
//
//void tick(FogMachine* fogMachine) {
//    static int state = 0;
//    static int count = 0;
//
//    switch (state) {
//        case (0):
//            //leave off for half a second
//            if (count > 3) {
//                state = 1;
//                count = 0;
//                fogMachine->start();
//            }
//
//            count++;
//        break;
//        case (1):
//            //turn on until it's show is over
//            if (fogMachine->canRelease()) {
//                state = 2;
//                count = 0;
//            }
//            count++;
//        break;
//        case (2):
//            //wait for a little to release
//            if (count > 5) {
//                state = 3;
//                count = 0;
//                fogMachine->release();
//            }
//
//            count++;
//        break;
//        case (3):
//            //wait for another release
//            if (fogMachine->canRelease()) {
//                state = 4;
//                count = 0;
//                fogMachine->stop();
//            }
//
//            count++;
//        break;
//        default:
//        break;
//    };
//}
//
//int main() {
//    printf("hello world\n");
//    FogMachine fogMachine = FogMachine();
//    fogMachine.setup();
//
//    while(true) {
//        tick(&fogMachine);
//        fogMachine.tick();
//        usleep(FSM_TICK_PERIOD_MS * 1000);
//    }
//    return 0;
//}
