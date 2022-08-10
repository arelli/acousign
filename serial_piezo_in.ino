const int vibrSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 30;  // threshold value to decide when the detected sound is a knock or not

int lastReadings[10];
int sensorReading;

// those need to be the same as in the tranceiver
// they need to bee 64-bit unsigned to accept time values
uint64_t const timeFactor = 40;
uint64_t globalInterval = 2 * timeFactor; // duration of tones
uint64_t globalPause = 0.5 * timeFactor; // duration between tones


uint64_t startTime;
uint64_t currentTime;
uint64_t lastSampleTime;
int counter = 0;
int const resolution = 100;  // how many readings per bit
int measurements[resolution];  // holds all the measurements of one bit time window(to be later averaged)

void setup() {
  Serial.begin(9600);       // use the serial port
}

void loop() {
  sensorReading = analogRead(vibrSensor);
  Serial.println(sensorReading);



  // demodulation

  if (sensorReading > threshold) { // the message has started
    // sample for globalInterval*2 to see if it is an actual starting tone
    lastSampleTime = currentTime = startTime = get_absolute_time()._private_us_since_boot;  // it returns a struct. See types.h of Pico
    while (currentTime < startTime + globalInterval * 2) {
      //sample the waveform 100 times in globalInterval*2 useconds
      if (absolute_time_diff_us(currentTime - lastSampleTime) < (globalInterval * 2) / 100) {
        sensorReading = analogRead(vibrSensor);
        measurements[counter] = sensorReading; counter += 1;
        lastSampleTime = get_absolute_time_us()._private_us_since_boot;
      }
      currentTime = get_absolute_time_us()._private_us_since_boot;
    }

    // first 8 bits are the length of the payload. Each bit lasts globalInterval + globalPause seconds


  }
  //delay(1);

}


