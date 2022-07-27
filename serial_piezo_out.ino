// with config of: 5 and 2 ms 50 8-bit characters take 5 sec to transmit

//int incomingByte = 0; // for incoming serial data
String incomingString;

int globalInterval = 5;
int globalPause = 2;
int piezoOutPin = 9;
int highFreq = 4500;
int lowFreq = 2000;
int bitsInAChar = 8;

/*
int globalInterval = 50;
int globalPause = 20;
int piezoOutPin = 9;
int highFreq = 600;
int lowFreq = 400;
int bitsInAChar = 8;
*/

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // send data only when you receive data:
  while (!Serial.available() > 0) {
    continue;  // execute the commads below only after receiving input from serial
  }
  // read the incoming byte
  incomingString = Serial.readString();
  sendPayload(incomingString);
  delay(globalPause);
}

void sendChar(int myChar){
  int characters[bitsInAChar];
  int andOp=1;
  for(int i=0; i<bitsInAChar; i++){ //  split the character to bits
    characters[i] = myChar & andOp;
    andOp= andOp*2;  // go to next bit
  }

  for(int i=0;i<bitsInAChar;i++){ // send the bits to output, with certain intervals!
    delay(globalInterval/5);  // delay between tones
    if(characters[i]>0){  // if the bitwise AND "fails"
      tone(piezoOutPin,highFreq);
      delay(globalInterval);
      noTone(piezoOutPin);
    }
    else{
      //tone(9,lowFreq); //  leave this uncommented to have "FSK"
      //noTone(piezoOutPin);  //  leave this line to have On-Off Keying
      delay(globalInterval);
      //noTone(piezoOutPin);
    }
  }
}


void initiatePacket(int packetSize){
  int characters[bitsInAChar];
  // split argument into bits
  for(byte i = 0; i < 8; i++){
    characters[i] = ((packetSize >> i) & 0x01);
  }
  
  tone(piezoOutPin,highFreq);  // synchronization tone
  delay(20);  // to differentiate this particular tone from other "bits"
  noTone(piezoOutPin);
  delay(globalInterval/5);
  
  for (int i=0; i<8; i++){
    if(characters[i]!=0){
      tone(piezoOutPin,highFreq);
    }
    else{
      tone(piezoOutPin,lowFreq);
    }
    delay(globalInterval);
    noTone(piezoOutPin);
    delay(globalInterval/5);  // delay between tones
  }
    
}

void sendPayload(String payloadString){
  initiatePacket(payloadString.length());  // tell the receiver how many bits are in the payload
  for(int i=0; i<payloadString.length(); i++){
    sendChar(payloadString.charAt(i));
  }
}
