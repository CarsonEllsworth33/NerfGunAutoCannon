// Created By Carson Ellsworth
// This code is to drive stepper motors and a servo motor
// These motors correspond to a trigger(servo) motion as well as pan and tilt(stepper) motion
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
 
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  200 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  700 // this is the 'maximum' pulse length count (out of 4096)

// Defines pins numbers
const int stepPin = 3;
const int stepPin2 = 5;
const int dirPin = 4; 
const int dirPin2 = 6;
const int joySw = 7;
int xJoy = A1;
int yJoy = A2;
int customDelay,customDelayMapped; // Defines variables
char serDat;
int intDat[2];
 
void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  pwm1.begin();
  pwm1.setPWMFreq(60);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  pinMode(stepPin2,OUTPUT);
  pinMode(xJoy,INPUT);
  pinMode(yJoy,INPUT);
  pinMode(joySw,INPUT);
  digitalWrite(dirPin,HIGH); //Enables the motor to move in a particular direction
  delay(10);
  Serial.println("Post Setup");
}



void loop() {
 
// Serial.print(digitalRead(joySw));
 if(serDat == 'g'){
    Serial.print("true");// Gets custom delay values from the custom speedUp function
    // Makes pules with custom delay, depending on the Potentiometer, from which the speed of the motor depends
    for(int j = 0; j < intDat[0]; j++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(intDat[1]);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(intDat[1]);
    }
 }
 if(serDat =='f'){
  Serial.println("fire");
    shoot();
  }

 //X going one way
 if(analogRead(xJoy) > 600){
    digitalWrite(dirPin,LOW);
    customDelayMapped = speedUpUp(xJoy);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(customDelayMapped);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(customDelayMapped);
  }

 //Y going one way
 if(analogRead(yJoy) > 600){
    digitalWrite(dirPin2,LOW);
    customDelayMapped = speedUpUp(yJoy);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(customDelayMapped);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(customDelayMapped);
  }

  //X going other way
  if(analogRead(xJoy) < 500){
    digitalWrite(dirPin,HIGH);
    customDelayMapped = speedUpDown(xJoy);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(customDelayMapped);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(customDelayMapped);
  }

  //Y going other way
  if(analogRead(yJoy) < 500){
    digitalWrite(dirPin2,HIGH);
    customDelayMapped = speedUpDown(yJoy);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(customDelayMapped);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(customDelayMapped);
  }
  

  
}

//shoot
 void shoot(){
      //Serial.print("joystick fire");
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm1.setPWM(0, 0, pulselen);
  }

  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm1.setPWM(0, 0, pulselen);
  }

  delay(500);
 }

void serialEvent(){
  serDat = Serial.read();
  //steps
  intDat[0] = Serial.parseInt();
  //speed
  intDat[1] = Serial.parseInt();
  
  //intDat[2] = Serial.parseInt();
  
  Serial.println(serDat);
  Serial.println(intDat[0]);
  Serial.println(intDat[1]);

}



// Function for reading the Potentiometer
int speedUpUp(int pinRead) {
  int customDelay = analogRead(pinRead); // Reads the potentiometer
  int newCustom = map(customDelay, 600, 1023, 4000,500); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  return newCustom;  
}


// Function for reading the Potentiometer
int speedUpDown(int pinRead) {
  int customDelay = analogRead(pinRead); // Reads the potentiometer
  int newCustom = map(customDelay,500, 0, 4000,500); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  return newCustom;  
}


void moveToPos(bool dir, float deg,int timeDelay){
  //200 steps = 1 rev & 1 step is 1.8 degrees
  //90 degree is 90/1.8 step
  deg = deg/360;
  digitalWrite(dirPin,dir);
  for(int i = 0; i < int(200*deg); i++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(timeDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(timeDelay);
    }
  }
