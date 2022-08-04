#include "avdweb_AnalogReadFast.h"

const uint8_t vibrSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 5;  // threshold value to decide when the detected sound is a knock or not

int elapsedTime = 0;
int startTime= 0;
int currentTime=0;

uint16_t peakCounter = 0;

uint16_t packetTime = 50;  // the time window of a packet. Used for better serial visualisation

// these variables will change:
uint16_t sensorValue = 0;      // variable to store the value read from the sensor pin
long unsigned sensorReading;


// smoothing
int counter = 0;


void setup() {
  Serial.begin(9600);       // use the serial port

  analogReference(INTERNAL1V1); // THIS is ONLY for Arduino MEGA! It sets the reference AREF at 1.1V(resolution of 1mv)
}

void loop() {

  sensorReading = analogReadFast(vibrSensor);
  if(sensorReading>threshold){
    startTime = millis();
    while(elapsedTime<packetTime){
      sensorReading += analogReadFast(vibrSensor);
      elapsedTime = millis()-startTime;      
      counter ++;
    }   

    Serial.println(sensorReading/counter);
    counter = 0;
    elapsedTime = 0;
  }
  
}

/*
int smoothRead(int pin){
  sensorValue = analogReadFast(pin);
  if(counter<5){
    smoothedValue = (sensorValue+99*lastValue)/200;
    lastValue = sensorValue;
  }
  else{
    counter = 0;
    smoothedValue = lastValue = sensorValue;
  }

  return smoothedValue;
}
*/
