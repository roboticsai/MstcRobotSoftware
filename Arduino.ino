#include <VarSpeedServo.h> 

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// DC motor parameter
#define enA 9
#define in1 14
#define in2 15

#define enB 3
#define in3 16
#define in4 17

int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin

int servo0 = 600;
int servo180 = 2100;
int inc = 20;
int posS = servo0;
int servoPin = 11;
int pulseInterval=2000;

int pos = 0; 
VarSpeedServo myservo; 
void setup() {
  myservo.attach(11); 
  myservo.write(0);
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(servoPin, OUTPUT);

  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

}

float curStepper1Pos = 0;
int stepperDel = 0;

void MoveStepper(int newStepper1Pos) {
      if(newStepper1Pos > curStepper1Pos) {
        while(curStepper1Pos < newStepper1Pos) {
          digitalWrite(DIR,LOW);
          digitalWrite(ENA,HIGH);
          digitalWrite(PUL,HIGH);
          //delayMicroseconds(stepperDel);
          digitalWrite(PUL,LOW);
          //delayMicroseconds(stepperDel);
          curStepper1Pos+=0.05625;
          //delayMicroseconds(10);
          //Serial.print("stepper motor pos="); Serial.print(curStepper1Pos);  Serial.print(","); Serial.println(newStepper1Pos); 

        }            
      }
      if(newStepper1Pos < curStepper1Pos) {
        while(curStepper1Pos > newStepper1Pos) {
          digitalWrite(DIR,HIGH);
          digitalWrite(ENA,HIGH);
          digitalWrite(PUL,HIGH);
          //delayMicroseconds(stepperDel);
          digitalWrite(PUL,LOW);
          //delayMicroseconds(stepperDel);
          curStepper1Pos-=0.05625; 
          //delayMicroseconds(10); 
          //Serial.print("stepper motor pos="); Serial.print(curStepper1Pos);  Serial.print(","); Serial.println(newStepper1Pos); 

        }            
      }
}

int vel = 150;
void MoveWheel(int aKeys0,int aKeys1,int aKeys2) {
   if(aKeys0 == -1) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
   }
   else if(aKeys0 == 22 && aKeys1 == -1 && aKeys2 == -1) {
      analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(1);
   }
   else if(aKeys0 == 18 && aKeys1 == -1 && aKeys2 == -1) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(1);
  } 
  else if(aKeys0 == 3 && aKeys1 == -1 && aKeys2 == -1) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1);
  } 
  else if(aKeys0 == 0 && aKeys1 == -1 && aKeys2 == -1) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(1);
  } 
  else if((aKeys0 == 22 || aKeys1 == 22 || aKeys2 == 22) && (aKeys0 == 3 || aKeys1 == 3 || aKeys2 == 3)) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, vel/2); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1);
  } 
  else if((aKeys0 == 22 || aKeys1 == 22 || aKeys2 == 22) && (aKeys0 == 0 || aKeys1 == 0 || aKeys2 == 0)) {
    analogWrite(enA, vel/2); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(1);
  } 
  else if((aKeys0 == 18 || aKeys1 == 18 || aKeys2 == 18) && (aKeys0 == 3 || aKeys1 == 3 || aKeys2 == 3)) {
    analogWrite(enA, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, vel/2); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(1);
  } 
  else if((aKeys0 == 18 || aKeys1 == 18 || aKeys2 == 18) && (aKeys0 == 0 || aKeys1 == 0 || aKeys2 == 0)) {
    analogWrite(enA, vel/2); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, vel); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(1);
  } 
  if((aKeys0 == 4 || aKeys1 == 4 || aKeys2 == 4) && vel < 250) {
    vel+=5;
  }
  else if((aKeys0 == 16 || aKeys1 == 16 || aKeys2 == 16) && vel > 0) {
    vel-=5;
  }
}

bool rev = false;
bool forw = false;
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
  // print the string when a newline arrives:
  if (stringComplete) {   
    // clear the string:
    Serial.print(inputString);
    int aKeys0 = inputString.substring(0,2).toInt();
    int aKeys1 = inputString.substring(2,4).toInt();
    int aKeys2 = inputString.substring(4,6).toInt();
    int mouseBut = inputString.substring(6,8).toInt();
    
    int newStepper1Pos = inputString.substring(8,11).toInt();
    int mouseY = inputString.substring(11,14).toInt();

    Serial.print(aKeys0); Serial.print(","); Serial.print(aKeys1); Serial.print(","); Serial.print(aKeys2); Serial.print(","); Serial.print(mouseBut); Serial.print(","); Serial.print(newStepper1Pos); Serial.print(","); Serial.println(mouseY);

    MoveStepper(newStepper1Pos);
    MoveWheel(aKeys0,aKeys1,aKeys2);
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

