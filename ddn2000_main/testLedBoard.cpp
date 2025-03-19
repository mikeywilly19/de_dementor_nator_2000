// #include <iostream>
// #include <unistd.h>
// #include "LedBoard.h"
// #include "globals.h"

// void tick(LedBoard* ledBoard) {
//    static int state = 0;
//    static int count = 0;

//    switch (state) {
//        case (0):
//            //leave off for half a second
//            if (count > 3) {
//                state = 1;
//                count = 0;
//                ledBoard->start();
//            }

//            count++;
//        break;
//        case (1):
//            //turn on until it's show is over
//            if (!ledBoard->isFlashing()) {
//                printf("show over\n");
//                state = 2;
//                count = 0;
//            }

//            count++;
//        break;
//        case (2):
//            //turn off (should already be off)
//        break;
//        default:
//        break;
//    };
// }

// int main() {
//    printf("hello world\n");
//    LedBoard ledBoard = LedBoard();
//    ledBoard.setup();

//    while(true) {
//        tick(&ledBoard);
//        ledBoard.tick();
//        usleep(FSM_TICK_PERIOD_MS * 1000);
//    }
//    return 0;
// }
