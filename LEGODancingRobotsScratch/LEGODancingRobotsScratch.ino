
/*LEGO Dancing Robots
 * connect buttons or other digital inputs to digital 2 and 3 using a pull down resistor as described here https://www.arduino.cc/en/tutorial/button
 * connect the infrared led to digital 12 as described here https://www.arduino.cc/en/Tutorial/Blink (connect to 12 instead of 13)
 * LEGOPowerFunctions library is found in the git. Online at http://basbase.com/robocam/LEGOPowerFunctions.zip Based on code of Bob Kojima 01-19-2008 further modified by Roland Wiersma
 * install library as usual: https://www.arduino.cc/en/Guide/Libraries
 * It looks like you need to replace "WProgram.h" with Arduino.h in the LEGOPowerfunctions library - I ought to do a PR for that. Meanwhile, just replace it:
 *http://forum.arduino.cc/index.php?topic=147680.0
 * 
 * version notes: 
 * - the timing of both inputs interfere, so erratic behaviour might occur. Using interrupts in improved version?
 * - only channel 1 is receiving input on red and blue to limit the time needed for the messages to be sent



*/


#include <legopowerfunctions.h>

LEGOPowerFunctions lego(12);

//Arbitrary commands for initiating communication
const byte PING = 0x64;
const byte PONG = 0x65;

const byte RED_CMD = 0x1;
const byte BLUE_CMD = 0x2;

int FWDspeed1 = 5;// here we can set speeds, for instance for the different channels?
int REVspeed1 = 11;
int FWDspeed2 = 5;
int REVspeed2 = 11;
int FWDspeed3 = 5;
int REVspeed3 = 11;
int FWDspeed4 = 5;
int REVspeed4 = 11;

boolean redFWD = false;  
boolean blueFWD = false;

boolean redMotorStopped = false;
boolean blueMotorStopped = false;

unsigned long redBeatTime = 0;
unsigned long blueBeatTime = 0;

int redPin = 2;
int bluePin = 3;
int movementDuration = 150000; //how long the motors should be on, measured in microseconds. I found 200000 to be a bit laggy response wise, but 1000000 not quite enough movement.

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  byte cmd;
  if (Serial.available() > 0) {
    cmd = Serial.read();
    if (cmd == PING) {
      Serial.write(PONG);
    } else if (cmd == RED_BEAT) {
      redBeat = true;
      redMotorStopped = false;
      moveRed();
    } else if (cmd == BLUE_BEAT) {
      digiblueBeat = true;
      blueMotorStopped = false;
      moveBlue();
    }
  }
  
  if(redBeatTime + movementDuration < micros() && !redMotorStopped){
    stopAllRedMotors();
    redMotorStopped = true;
  }

  if(blueBeatTime + movementDuration < micros() && !blueMotorStopped){
    stopAllBlueMotors();
    blueMotorStopped = true;
  }
  
  delay(1);//not sure if this is needed
}


void moveRed() {
    unsigned long startSend = micros();
    digitalWrite(13, redFWD);
    if(redFWD){
      lego.SingleOutput(PWM, FWDspeed1, RED, CH1);
//      lego.SingleOutput(PWM, FWDspeed2, RED, CH2); // other channels currently commented out to limit blocking of cycle
//      lego.SingleOutput(PWM, FWDspeed3, RED, CH3);
////      lego.SingleOutput(PWM, FWDspeed4, RED, CH4);
      
    }else{
      lego.SingleOutput(PWM, REVspeed1, RED, CH1);
//      lego.SingleOutput(PWM, REVspeed2, RED, CH2);
//      lego.SingleOutput(PWM, REVspeed3, RED, CH3);
//      lego.SingleOutput(PWM, REVspeed4, RED, CH4);
    }
    redFWD = !redFWD;
    redBeatTime = micros();

    Serial.print("start: ");
    Serial.println(micros()-startSend);
   
 
}

void moveBlue() {

    if(blueFWD){
      lego.SingleOutput(PWM, FWDspeed1, BLUE, CH1);
//      lego.SingleOutput(PWM, FWDspeed2, BLUE, CH2);
//      lego.SingleOutput(PWM, FWDspeed3, BLUE, CH3);
//      lego.SingleOutput(PWM, FWDspeed4, BLUE, CH4);
    }else{
      lego.SingleOutput(PWM, REVspeed1, BLUE, CH1);
//      lego.SingleOutput(PWM, REVspeed2, BLUE, CH2);
//      lego.SingleOutput(PWM, REVspeed3, BLUE, CH3);
//      lego.SingleOutput(PWM, REVspeed4, BLUE, CH4);
    }
    blueFWD = !blueFWD;
    blueBeatTime = micros();
 
}

void stopAllRedMotors(){
  unsigned long startSend = micros();
  lego.SingleOutput(PWM, 8, RED, CH1);
//  lego.SingleOutput(PWM, 8, RED, CH2);
//  lego.SingleOutput(PWM, 8, RED, CH3);
//  lego.SingleOutput(PWM, 8, RED, CH4);
  Serial.print("stop: ");
  Serial.println(micros()-startSend); 
}

void stopAllBlueMotors(){
  lego.SingleOutput(PWM, 8, BLUE, CH1);
//  lego.SingleOutput(PWM, 8, BLUE, CH2);
//  lego.SingleOutput(PWM, 8, BLUE, CH3);
//  lego.SingleOutput(PWM, 8, BLUE, CH4);
}


