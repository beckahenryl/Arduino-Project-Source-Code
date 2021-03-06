///////////////////////////////////////////
// Program Name: <Choice of User with Serial>
// Program Version: 1.0
// Author: <Rebeka>
// Date: <10/16/18>
// Inputs to Program: <M, L, P>
// Outputs from Program: <None>
////////////////////////////////////////// 

#include <Servo.h>

int pinL = 8;                  //pin to control LED Red
int pinL1 = 7;                //pin to control LED Green
int pinP = 6;                  //pin to control Photoresistor and Clear LED 
int pinP1 = 5;                //pin to control Photoresistor and Blue LED
int position_servo = 0;  //starting position of servo 
Servo myMotor;

void setup() {
/* interactive choices with the different pins
 * each of the commands in the for loop below
 * will be able to control the output that is 
 * being set up here
 */
    pinMode(pinL,OUTPUT);
    pinMode(pinL1,OUTPUT);
    pinMode(pinP,OUTPUT);
    pinMode(pinP1,OUTPUT);
    myMotor.attach(9);       //pin number 9 for servo motor
    Serial.begin(9600);
    Serial.println("Choose Action: M-Motor, L-LED, P- LED with Photoresistor Command");
}

void loop() {
/* use if else to go through each condition 
 * on what the user inputed from serial
 * use for loop to change the angle of the 
 * motor arm of the servo it does through 
 * a 180 degree turn waits and then turns 
 * the other direction
 */
  if (Serial.available ()){ //checks if we have access to serial monitor and reads the monitor below
      char get_user_in = Serial.read();
      if (get_user_in == 'M'){ //if user inputs an 'M' then the motor will be the first to run
        for (position_servo = 0; position_servo <= 180; position_servo+=1){
            myMotor.write(position_servo);
            delay(15);
          }
          delay(1000);
          for (position_servo = 180; position_servo >= 0; position_servo-=5){
              myMotor.write(position_servo);
              delay(15);
            }
            delay(1000);
    }
    else if (get_user_in == 'L'){      //if user inputs an 'L' the Red and Green LED will light up first
        digitalWrite(pinL, HIGH);     //turn on LED Red
        digitalWrite (pinL1, HIGH);  //turn on LED Green
        delay (1000);
    }
    else if (get_user_in == 'P'){     //if user input 'P' the Clear and Blue LED will light up
        digitalWrite(pinP, HIGH);    //turn on Clear LED when it is light, turn off LED when it is dark
        digitalWrite (pinP1, HIGH); //turn on Blue LED when it is light, turn off LED when it is dark
        delay (1000);
    }
    else{
        Serial.println(" ");        //prints nothing if conditions above are not met
    }
  }
}