const int vibrSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 10;  // threshold value to decide when the detected sound is a knock or not

int elapsedTime = 0;
int startTime= 0;
int currentTime=0;

int packetTime = 500;  // the time window of a packet. Used for better serial visualisation

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin

void setup() {
  Serial.begin(115200);       // use the serial port
}

void loop() {
  sensorReading = analogRead(vibrSensor);
  if(sensorReading>1){
    startTime = millis();
    while(elapsedTime<packetTime){
      sensorReading = analogRead(vibrSensor);
      Serial.println(sensorReading);
      elapsedTime = millis()-startTime;      
    }    
    elapsedTime = 0;
  }
}
