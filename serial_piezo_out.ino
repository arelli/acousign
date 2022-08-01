/* Arduino underwater acoustic modem Transmitter Module
 * With the code below, we can send bytes acoustically to other listening devices. 
 * It decodes each byte to its bits, and sends them using On-Of keying(ASK). it is 
 * A form of Amplitude Modulation, and it is chosen because of the strange behavior
 * of acoustic wave phases underwater(attenuation, multi-path propagation etc). 
 * Currently being tested inside a water bottle, with minaml distance of about 5cm 
 * between the two piezoelectric transducers(waterproofed. DIY Hydrophones.
 */

String incomingString;

int globalInterval = 1;
int globalPause = 4;
int piezoOutPin = 9;
int highFreq = 50000;
//int lowFreq = 2500;
int bitsInAChar = 8;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // Test String with the press of RESET Button
  sendPayload("abcdefg");
  delay(globalPause);
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
    delay(globalPause);  // delay between tones
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
  delay(8);  // to differentiate this particular tone from other "bits"
  noTone(piezoOutPin);
  delay(globalPause);
  
  for (int i=0; i<8; i++){
    if(characters[i]!=0){
      tone(piezoOutPin,highFreq);
      delay(globalInterval);
    }
    else{
      //tone(piezoOutPin,lowFreq);
      delay(globalInterval);
      
    }
    noTone(piezoOutPin);
    delay(globalPause);  // delay between tones
  }
    
}

void stopPacket(){
  tone(piezoOutPin,highFreq);  // end-of-message tone
  delay(8);  // to differentiate this particular tone from other "bits"
  noTone(piezoOutPin);
}

void sendPayload(String payloadString){
  initiatePacket(payloadString.length());  // tell the receiver how many bits are in the payload
  for(int i=0; i<payloadString.length(); i++){
    sendChar(payloadString.charAt(i));
  }
  stopPacket();
}
