const int vibrSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 5;  // threshold value to decide when the detected sound is a knock or not

int elapsedTime = 0;
int startTime= 0;
int currentTime=0;

uint16_t peakCounter = 0;

uint16_t packetTime = 500;  // the time window of a packet. Used for better serial visualisation

// these variables will change:
uint16_t sensorReading = 0;      // variable to store the value read from the sensor pin

// next are the variables for smoothing
const uint8_t numReadings = 5;

uint16_t readings[numReadings];      // the readings from the analog input
uint16_t readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
uint16_t average = 0;                // the average

uint8_t inputPin = A0;

unsigned long startTime2;
unsigned long elapsedTime2;

void setup() {
  Serial.begin(9600);       // use the serial port
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  analogReference(INTERNAL1V1); // THIS is ONLY for Arduino MEGA! It sets the reference AREF at 1.1V(resolution of 1mv)
}

void loop() {
  startTime2=millis();
  
  sensorReading = analogRead(vibrSensor);
  if(sensorReading>1){
    startTime = millis();
    while(elapsedTime<packetTime){
      sensorReading = smoothingFunc();
      
      //Serial.println(sensorReading);
      
      elapsedTime = millis()-startTime;      
    }    
    elapsedTime = 0;
  }
  elapsedTime2 = millis()-startTime2;
  Serial.println(elapsedTime2);
}


int smoothingFunc(){
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(vibrSensor);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  return average;
}
