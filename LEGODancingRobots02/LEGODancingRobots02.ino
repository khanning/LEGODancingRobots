/*LEGO Dancing Robots
 * connect buttons or other digital inputs to digital 2 and 3 using a pull down resistor as described here https://www.arduino.cc/en/tutorial/button
 * connect the infrared led to digital 12 as described here https://www.arduino.cc/en/Tutorial/Blink (connect to 12 instead of 13)
 * LEGOPowerFunctions library is found in the git. Online at http://basbase.com/robocam/LEGOPowerFunctions.zip Based on code of Bob Kojima 01-19-2008 further modified by Roland Wiersma
 * install library as usual: https://www.arduino.cc/en/Guide/Libraries
 * 
 * 
 *
 * - only channel 1 is receiving input on red and blue 



*/


#include <legopowerfunctions.h>         //add the LEGO Power Function library to the code

#define PWM_FLT 0x0                     //the speed values to send to the Power Function receiver
#define PWM_FWD1 0x1
#define PWM_FWD2 0x2
#define PWM_FWD3 0x3
#define PWM_FWD4 0x4
#define PWM_FWD5 0x5
#define PWM_FWD6 0x6
#define PWM_FWD7 0x7
#define PWM_BRK 0x8
#define PWM_REV7 0x9
#define PWM_REV6 0xA
#define PWM_REV5 0xB
#define PWM_REV4 0xC
#define PWM_REV3 0xD
#define PWM_REV2 0xE
#define PWM_REV1 0xf

LEGOPowerFunctions lego(12);              //set the output for the Power Functions to pin 12

int redPin = 2;                           //the digital input pin that controls every motor connected to RED
int bluePin = 3;                          //the digital input pin that controls every motor connected to BLUE

int FWDspeed = PWM_FWD5;                  //forward speed 
int REVspeed = PWM_REV5;                  //reverse speed


boolean redIsMoving= false;                //is every motor connected to RED moving?
boolean blueIsMoving= false;               //is every motor connected to BLUE moving?

boolean redIsMovingForward = false;        //is every motor connected to RED moving in the FORWARD direction?
boolean blueIsMovingForward = false;       //is every motor connected to BLUE moving in the FORWARD direction?

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  
  if(digitalRead(redPin) == HIGH && !redIsMoving){                  // MOVE RED when the input for RED is activated and RED is not yet moving:
    redIsMoving = true;                                             // set the variable to 'moving' so it only triggers once
    moveRed();                                                      // and move every motor connected to RED.
    
  }else if(digitalRead(redPin) == LOW && redIsMoving){              // STOP RED when the input for RED is no longer activated but RED is still moving:
    redIsMoving = false;                                            // set the variable to 'not moving' so it only triggers once
    lego.SingleOutput(PWM, PWM_BRK, RED, CH1);                      // and stop every motor connected to RED.
  }

 
 if(digitalRead(bluePin) == HIGH && !blueIsMoving){                 // MOVE BLUE when the input for BLUE is activated and BLUE is not yet moving:
    blueIsMoving = true;                                            // set the variable to 'moving' so it only triggers once
    moveBlue();                                                     // and move every motor connected to BLUE.
    
  }else if(digitalRead(bluePin) == LOW && blueIsMoving){            // STOP BLUE when the input for BLUE is no longer activated but BLUE is still moving:
    blueIsMoving = false;                                           // set the variable to 'not moving' so it only triggers once
    lego.SingleOutput(PWM, PWM_BRK, BLUE, CH1);                     // and stop every motor connected to BLUE.
  }
  
  
}


void moveRed() {
  if(redIsMovingForward){                                           // If motors connected to RED are moving forward,
    lego.SingleOutput(PWM, REVspeed, RED, CH1);                     // set them to move in reverse.
  }else{                                                            // If they are moving in reverse,
    lego.SingleOutput(PWM, FWDspeed, RED, CH1);                     // set them to move forward.
  }
  redIsMovingForward = !redIsMovingForward;                         // Flip the direction for the next move so the robots move back and forth.
}

void moveBlue() {
  if(blueIsMovingForward){                                          // If motors connected to BLUE are moving forward,
    lego.SingleOutput(PWM, REVspeed, BLUE, CH1);                    // set them to move in reverse.
  }else{                                                            // If they are moving in reverse,
    lego.SingleOutput(PWM, FWDspeed, BLUE, CH1);                    // set them to move forward.
  }
  blueIsMovingForward = !blueIsMovingForward;                       // Flip the direction for the next move so the robots move back and forth.
}




