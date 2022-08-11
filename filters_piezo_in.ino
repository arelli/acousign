// This file shows an example implementation of a 
// second order low-pass Butterworth filter on an Arduino. 
// Note that there are many possible improvements to this code.



const int vibrSensor = A0;

// Butterworth filter variables
float x[] = {0,0,0};
float y[] = {0,0,0};
int k = 0;

// Smoothing Variables
const uint8_t numReadings = 10;  // over how many readings to average

uint16_t readings[numReadings];      // the readings from the analog input
uint16_t readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
float average = 0;                // the average

uint8_t inputPin = A0;


void setup() {
  Serial.begin(115200);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // Test signal
  float t = micros()/1.0e6;
  x[0] = analogRead(vibrSensor);  // sin(2*PI*2*t) + 0.5*sin(2*PI*25*t);

  // Compute the filtered signal
  // (second order Butterworth example)
  float b[] = {0.00024132, 0.00048264, 0.00024132};
  float a[] = {1.95558189, -0.95654717};
  y[0] = a[0]*y[1] + a[1]*y[2] +
               b[0]*x[0] + b[1]*x[1] + b[2]*x[2];

  if(k % 3 ==0)
  {
    // This extra conditional statement is here to reduce
    // the number of times the data is sent through the serial port
    // because sending data through the serial port
    // messes with the sampling frequency
    
    // For the serial monitor
    Serial.print(2*x[0]);  // plot the original signal
    Serial.print(" ");  
    Serial.print(2*y[0]);  // plot the filtered signal with Butterworth filter of second order(https://www.youtube.com/watch?v=HJ-C4Incgpw)
    Serial.print(" ");
    Serial.println(smoothingFunc());  // plot using normal time domain smoothing
  }

  delay(5); // Slow down the plot!
  // butterworth filter stuff
  for(int i = 1; i >= 0; i--){
    x[i+1] = x[i]; // store xi
    y[i+1] = y[i]; // store yi
  }
  
  k = k+1;
}



// simple smoothing time domain
float smoothingFunc(){
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
