#include "Speaker.h"
SoftwareSerial mySerial(10, 11);


void Speaker::setup() {

 pinMode(buttonPause, INPUT);
 digitalWrite(buttonPause,HIGH);
 // put your setup code here, to run once:
 mySerial.begin (9600);
 delay(3000);

 setVolume(VOLUME);
 
 isPlaying = false;
 pause();
}

void Speaker::start() {
 if(!isPlaying){
   isStarted = true;
 }
}

void Speaker::tick() {
 // put your main code here, to run repeatedly:

 //Transitions
 if (isPlaying) {
   if (endOfPlay >= millis()) {
     pause();
     isPlaying = false;
   }
 } else {
   //!isPlaying
   if (isStarted) {
     isPlaying = true;
     isStarted = false;
     playFirst();
     endOfPlay = millis() + PLAY_TIME_MS;
   }
 }
}

void Speaker::playFirst()
{
 execute_CMD(0x3F, 0, 0);
 delay(500);
 execute_CMD(0x11,0,1); 
 delay(500);
}

bool Speaker::getIsPlaying() {
 return isPlaying;
}


void Speaker::play()
{
 if (isPlayFirst){
   playFirst();
 }
 else{
   execute_CMD(0x0D,0,1); 
   delay(500);
 }
}

void Speaker::pause()
{
 execute_CMD(0x0E,0,0);
 delay(500);
}

void Speaker::playNext()
{
 execute_CMD(0x01,0,1);
 delay(500);
}

void Speaker::setVolume(int volume)
{
 execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
 delay(2000);
}

void Speaker::execute_CMD(uint8_t CMD, uint8_t Par1, uint8_t Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
uint8_t Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (uint8_t k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
