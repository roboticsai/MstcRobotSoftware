bool is_last_line = false; // for incoming serial data
String readString;

void setup() {
  Serial.begin(2000000); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
//  if (Serial.available() > 0) {
//    // read the  byte:
//    incomingByte = Serial.read();
//    Serial.print("I received: ");
//    Serial.println(incomingByte);
//  }
  while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill
      if (Serial.available()) {
        char c = Serial.read();  //gets one byte from serial buffer
        //int value = (int)c-48;
        //Serial.println(c);
        //if(c=='n') is_last_line = true;
        readString += c; //makes the string readString
      }
  }
  if (readString.length() >0) {
    Serial.println(readString);
    readString = "";
    is_last_line = false;
  }
}

