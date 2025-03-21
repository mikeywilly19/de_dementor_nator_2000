#include "Speaker.h"

/***************************************************
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the all the function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ******AllTopic:All**********************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
<https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

void Speaker::setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  FPSerial.begin(9600);
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    digitalWrite(LED_BUILTIN, HIGH);
    while(true);
  }
  digitalWrite(LED_BUILTIN, LOW);
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(20);  //Set volume value (0~30).
  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down


  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
    
    //----Set device we use SD as default----
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  // myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
    
    //----Mp3 control----
  //  myDFPlayer.sleep();     //sleep
  //  myDFPlayer.reset();     //Reset the module
  //  myDFPlayer.enableDAC();  //Enable On-chip DAC
  //  myDFPlayer.disableDAC();  //Disable On-chip DAC
  //  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15

  //----Mp3 play----
   //myDFPlayer.next();  //Play next mp3
  // myDFPlayer.previous();  //Play previous mp3
  // delay(1000);
  // myDFPlayer.play(1);  //Play the first mp3
  // delay(9000);
  // myDFPlayer.loop(1);  //Loop the first mp3
  // delay(1000);
  //myDFPlayer.pause();  //pause the mp3

  // myDFPlayer.start();  //start the mp3 from the pause
  // delay(1000);
  // myDFPlayer.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  // delay(1000);
  // myDFPlayer.enableLoopAll(); //loop all mp3 files.
  // delay(1000);
  // myDFPlayer.disableLoopAll(); //stop loop all mp3 files.
  // delay(1000);
  // myDFPlayer.playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  // delay(1000);
  // myDFPlayer.advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
  // delay(1000);
  // myDFPlayer.stopAdvertise(); //stop advertise
  // delay(1000);
  // myDFPlayer.playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  // delay(1000);
  // myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
  // delay(1000);
  // myDFPlayer.randomAll(); //Random play all the mp3.
  // delay(1000);
  // myDFPlayer.enableLoop(); //enable loop.
  // delay(1000);
  // myDFPlayer.disableLoop(); //disable loop.
  // delay(1000);

  //----Read imformation----
}

void Speaker::start() {
  isStarted = true;
}

void Speaker::stop() {
  //stop the state machine
}

bool Speaker::isPlaying() {
  return state == 1;
}


void Speaker::tick()
{
  switch(state){
    case 0:
      // OFF
      //transition
        if(isStarted == true){
          isStarted = false;
          if (first){
            myDFPlayer.next();
            delay(100);
            myDFPlayer.next();
            delay(100);
            myDFPlayer.next();
            delay(100);
            myDFPlayer.next();
            delay(100);
            first = false;
          }
          else{
            myDFPlayer.next();
            delay(100);
          }
          state = 1;
          count = 0;
          digitalWrite(LED_BUILTIN, HIGH);
        }
      //actions
        //do nothing
      break;
    case 1:
      // ON
      //transition
      if (count >= 90){
        state = 0;
        myDFPlayer.pause();
        digitalWrite(LED_BUILTIN, LOW);
        isStarted = false;
      }
      //actions
      count += 1;
      break;

  }


// uint32_t startTime = millis();
// while(startTime + FSM_TICK_PERIOD_MS > millis()){

// }



  // static unsigned long timer = millis();
  // buttonState = digitalRead(buttonPin);  // Read the state of the button
  
  // if ((millis() - timer > 9000) && (buttonState == HIGH)) {
  //   timer = millis();
  //   myDFPlayer.next();
  //   timesPlayed += 1;
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  // else if((millis() - timer > 9000) && (buttonState == LOW)) {
  //   myDFPlayer.pause();
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }

  
}
