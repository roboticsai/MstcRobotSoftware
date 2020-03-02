#include <Servo.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// DC motor parameter
#define enA 9
#define in1 3
#define in2 2

int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin

Servo myservo;
int pos = 0; 

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  myservo.attach(11);
  myservo.write(0);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

int curStepper1Pos = 320;
int prevStepper1Pos = 320;
int stepperDel = 0;

void MoveStepper(int newStepper1Pos) {
  Serial.print("stepper pos="); Serial.println(newStepper1Pos);
      prevStepper1Pos = newStepper1Pos;
      if(newStepper1Pos > curStepper1Pos) {
        while(curStepper1Pos != newStepper1Pos && newStepper1Pos == prevStepper1Pos) {
          digitalWrite(DIR,LOW);
          digitalWrite(ENA,HIGH);
          digitalWrite(PUL,HIGH);
          delayMicroseconds(stepperDel);
          digitalWrite(PUL,LOW);
          delayMicroseconds(stepperDel);
          curStepper1Pos++;
          //Serial.print(curStepper1Pos); Serial.print("---"); Serial.println(newStepper1Pos);  
        }            
      }
      if(newStepper1Pos < curStepper1Pos) {
        while(curStepper1Pos != newStepper1Pos && newStepper1Pos == prevStepper1Pos) {
          digitalWrite(DIR,HIGH);
          digitalWrite(ENA,HIGH);
          digitalWrite(PUL,HIGH);
          delayMicroseconds(stepperDel);
          digitalWrite(PUL,LOW);
          delayMicroseconds(stepperDel);
          curStepper1Pos--;  
          //Serial.print(curStepper1Pos); Serial.print("---"); Serial.println(newStepper1Pos);  
        }            
      }
}

int vel = 150;
void MoveWheel(int key) {
  Serial.print("move wheel");
  Serial.println(key);
  if(key == -1) {
      analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
  }
  if(vel >=0 && vel <= 255) {
     if(key == 22) {
      analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay(1000);
    }
    else if(key == 18) {
      analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      delay(1000);
    } 
  }
}

void MoveServo() {
    Serial.println("Trigger pulled-----------------------------------------");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(2);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(2);                       // waits 15ms for the servo to reach the position
    }
}

bool mouseButReleased = false;
bool mouseButPressed = false;
void loop() {
        analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay(1000);
  // print the string when a newline arrives:
  if (stringComplete) {   
    // clear the string:
    //Serial.print(inputString);
    int aKeys0 = inputString.substring(0,2).toInt();
    int aKeys1 = inputString.substring(2,4).toInt();
    int aKeys2 = inputString.substring(4,6).toInt();
    int mouseBut = inputString.substring(6,8).toInt();
    int newStepper1Pos = inputString.substring(8,12).toInt();
    int mouseY = inputString.substring(12,16).toInt();

    Serial.print(aKeys0); Serial.print(aKeys1); Serial.print(aKeys2); Serial.print(mouseBut); Serial.print(newStepper1Pos); Serial.println(mouseY);

    MoveStepper(newStepper1Pos);
    if(vel >=0 && vel <= 255) {
     if(aKeys0 == 22) {
        Serial.print("move wheel");
        Serial.println(aKeys0);
        analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        delay(1000);
     }
    }
    if(mouseBut == 1) {
      mouseButPressed = true;      
    }
    if(mouseButPressed == true && mouseBut == -1) {
      mouseButPressed = false;
      mouseButReleased = true;
    }
    if(mouseButReleased == true) {
      MoveServo();
      mouseButReleased = false;
    }
    
    inputString = "";
    stringComplete = false;
  } 
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()>0) {
    // get the new byte:
    char inChar = Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
