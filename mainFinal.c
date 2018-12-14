
///////////////////////////////////////////
// Program Name: <Burn the Forest?>
// Program Version: 1.0
// Author: <Rebeka Henry>
// Date: <12/10/2018>
// Inputs to Program: <Y,N>
// Outputs from Program:<temperature>
////////////////////////////////////////// 

#include <LiquidCrystal.h>
#include <EEPROM.h>

/*LEDs initialization*/

//forest
int greenLED = A2;
//fire!
int redLED = A3; 

/*Piezo initialization*/

int sensorValue;
// variable to calibrate low value
int sensorLow = 1023;
// variable to calibrate high value
int sensorHigh = 0;

/*Temperature Sensor initialization*/

// named constant for the pin the sensor is connected to
const int sensorPin = A5;
// room temperature in Celsius
const float baselineTemp = 20.0;

int blueLEDTemp = A1;

/*Motor initialization*/

//switch input pin for motor
const int switchPin = 6;
// connected to pin 7 on the H-bridge     
const int controlPin1 = 10;  
// connected to pin 2 on the H-bridge
const int controlPin2 = 7;  
// connected to pin 1 on the H-bridge 
const int enablePin = 9;    

/*LCD initialization*/

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*EEPROM initialization*/

int address = 0;
byte readValue;

void setup() {

  Serial.begin(9600);

  Serial.print("Enter Forest");
  

  /*LEDs*/

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  //turn on green LED
  digitalWrite(greenLED, HIGH);

  /*piezo*/

  //read the input from A0 and store it in a variable
  sensorValue = analogRead(A0);

  // map the sensor values to a wide range of pitches
  int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 4000);

  // play the tone for 20 ms on pin 8
  tone(8, pitch, 20);

  // wait for a moment
  delay(10);
  
  // calibrate for the first five seconds after program runs
  while (millis() < 5000) 
  { 
    // record the maximum sensor value
    sensorValue = analogRead(A0); 
    if (sensorValue > sensorHigh)
    {
      sensorHigh = sensorValue;
    }
    // record the minimum sensor value
    if (sensorValue < sensorLow)
    {
      sensorLow = sensorValue;
    }
  }

  /*Temperature Sensor*/

  pinMode(blueLEDTemp, OUTPUT);
  //turn blue LED to indicate circuit is working
  digitalWrite(blueLEDTemp, HIGH);

  /*Motor*/

  //set the switch pin as input
  pinMode(switchPin, INPUT);

  //set other motor pins as outputs
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  //to be able to turn on the motor
  digitalWrite(enablePin, HIGH);
  
  //serial communication
  Serial.println("Do you burn the forest? Y/N");
}
 

void loop() {

  char burnForest = Serial.read();

    if (burnForest == 'Y')
    {
      //red represents the fire
      digitalWrite(redLED, HIGH); 

      //piezo represents the animals screeching
      sensorValue = analogRead(A0); 

      // map the sensor values to a wide range of pitches
      int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 4000);

      // play the tone for 20 ms on pin 8
      tone(8, pitch, 20);

      // wait for a moment
      delay(10);

      lcd.clear(); //LCD displays fire to represent this command
      // set the cursor to column 0, line 0
      lcd.setCursor(0, 0);
      // print some text
      lcd.print("Fire!!!!");
      // move the cursor to the second line
      lcd.setCursor(0, 1);

    }

    else if (burnForest == 'N')
    {
      //no Fire
      digitalWrite(redLED, LOW); 

       //temperature sensor is turned on
      int sensorVal = analogRead(sensorPin);

      // send the 10-bit sensor value out the serial port
      Serial.print("sensor Value: ");
      Serial.print(sensorVal);
      //store value into EEPROM
      EEPROM.write(address, sensorVal);

      //read the stored value after restarting the program
      readValue = EEPROM.read(address);
      Serial.print(address);
      Serial.print("\t");
      Serial.print(readValue, DEC);
      Serial.println();

      address += 1;

      if (address == EEPROM.length())
      {
        address = 0;
      }

      delay(100);

      // convert the ADC reading to voltage
      float voltage = (sensorVal / 1024.0) * 5.0;

      // Send the voltage level out the Serial port
      Serial.print(", Volts: ");
      Serial.print(voltage);

      // convert the voltage to temperature in degrees C
      // the sensor changes 10 mV per degree
      // the datasheet says there's a 500 mV offset
      // ((voltage - 500 mV) times 100)

      Serial.print(", degrees C: ");
      float temperature = (voltage - .5) * 100;
      Serial.println(temperature);

      //LCD displays No fire, Wind
      lcd.clear(); 
      // set the cursor to column 0, line 0
      lcd.setCursor(0, 0);
      // print some text
      lcd.print("It's Windy!");
      // move the cursor to the second line
      lcd.setCursor(0, 1);

      //motor turns on

      if (digitalRead(switchPin) == HIGH)
      {
        digitalWrite(controlPin1, LOW);
        digitalWrite(controlPin2, HIGH);
      }

      else 
      {
        digitalWrite(controlPin1, HIGH);
        digitalWrite(controlPin2, LOW);
      }
    }
  }
