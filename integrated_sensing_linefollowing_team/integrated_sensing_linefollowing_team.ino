long startTime = millis();

#include <Servo.h>

//Team Code
int teamResults[4];
int minIndex = 0; 
char finalRoutine;

//Line Sensing
Servo left;
Servo right;
int QTIpins[] = {49, 51, 53}; // left, center, right
int QTIdurations[3];
int QTIvalues[3];

//Sensing
int threshold_qti = 100;
int count = 0;
const int pingPin = 7;
int tall_block_count = 0;        
char outgoing;
long threshold_Ping = 200;

//Communication
#include<SoftwareSerial.h>
#define Rx 17
#define Tx 16

void setup() {
  left.attach(11); //Attach left servo
  right.attach(12); //Attach right servo

  //Sensing
  pinMode(45, OUTPUT);//on-board LED
  pinMode(46, OUTPUT);//on-board LED

  //LCD
  Serial3.begin(9600);
  Serial3.write(12); //clear LCD screen
  Serial3.write(13); //clear LCD screen

  //Communication
  Serial2.begin(9600);

  digitalWrite(46, HIGH);//turn off on-board LED
  digitalWrite(45, HIGH);//turn off on-board LED
}

void loop() {
  checkSensors(); // if left black, turn left; if right black, turn right; if all three, increment hashmarks

  if (QTIvalues[0] and not QTIvalues[2]) { // if left
    left.write(90); // left backwards
    right.write(90); // right forwards
  }
  else if (QTIvalues[2] and not QTIvalues[0]) {
    left.write(96); // left forwards
    right.write(96); // right backwards
  }
  else {
    if(QTIvalues[0] and QTIvalues[1] and QTIvalues[2]) {
      count++;
      stopBot();

      pingSense();
      delay(500);

      if(count == 5){ //if the robot is on the fifth hash
        stopBot();
        left.detach();
        right.detach();
        calculateOutgoing(); //update char outgoing based off of tall_block_count
          
        for(int i = 0;  i < 2; i++) {
            Serial2.write(outgoing);            
        }
        teamResults[3] = tall_block_count; //our robot calculation

        //Team code
        while(!receiveCharacter()) {
        }
        compute();
        if(minIndex == 0) {Serial3.write("1 is lowest.");}
        if(minIndex == 1) {Serial3.write("2 is lowest.");}
        if(minIndex == 2) {Serial3.write("3 is lowest.");}
        if(minIndex == 3) {Serial3.write("4 is lowest.");}
        while(!receiveFinalRoutine()) {
        }
        if(finalRoutine == 'x') {Serial3.write("FRX");}
        if(finalRoutine == 'y') {Serial3.write("FRY");}
        if(finalRoutine == 'z') {Serial3.write("FRZ");}        
      }
      goStraight();
      delay(200);
    }
    goStraight();
  }
}

void sendCharacter(char c) {
  char outgoing = c;
  Serial2.write(outgoing);
  digitalWrite(45, LOW);
  delay(50);
}

boolean receiveFinalRoutine() { //Team Code
  if(Serial2.available()) {
    char incoming = Serial2.read();
    if(incoming >= 'x' && incoming <= 'z') {
      finalRoutine = incoming;
      return true;
    }
    return false;
  }
  return false;
}

//Team Code
boolean receiveCharacter() {
  // Team 1: a-e
  // Team 2: f-j
  // Team 3: k-o
  // Team 4: p-t
  // Team 5: not needed since team 5 computes.
  if (millies() - startTime > 60000) {
    return true;
  }
  if(Serial2.available()){
    char incoming = Serial2.read();

    if(incoming >= 'a' && incoming <= 'e') {
      teamResults[0] = incoming-96;
    }
    if(incoming >= 'f' && incoming <='j') {
      teamResults[1] = incoming-101;
    }
    if(incoming >= 'k' && incoming <='o') {
      teamResults[2] = incoming-106;
    }
    for(int i = 0; i < 4; i++) {
      // If some data not yet received, return false.
      if(teamResults[i] == 0) {return false;}
    }
    // If we have all data, return true;
    return true;
  }
  return false;
}

//Team Code
void compute() {
  for(int i = 0; i < 4; i++) {
    if(teamResults[i] < teamResults[minIndex] && teamResults[i] != 0) {
      minIndex = i;
    }
  }
}

/* 
 * INPUT:  Reads the QTIvalues from the QTI sensors stored in the array QTIpins[]
 * OUTPUT: A 0 or 1 in array QTIvalues[] depending on if the sensor reads a dark or light objects 
 * From: https://github.com/erisawesome/QTI-Sensor-Demo/blob/master/QTI-Sensor-Demo.ino
 */
void checkSensors() {
  for(int i = 0; i < sizeof(QTIpins)/2; i++) { 
    long duration = 0;                      //
    pinMode(QTIpins[i], OUTPUT);               // code that actually    
    digitalWrite(QTIpins[i], HIGH);            // reads from the sensor 
    delay(1);                               // 
    pinMode(QTIpins[i], INPUT);                // low QTIvalues from the sensor
    digitalWrite(QTIpins[i], LOW);             // are dark objects, high
    while(digitalRead(QTIpins[i])){            // QTIvalues are light objects
      duration++;                           //
    }                                       
    QTIdurations[i] = duration;                // store QTIvalues in arrays
    //Serial.println(QTIdurations[0] + QTIdurations[1]+QTIdurations[2]);
    QTIvalues[i] = (duration > threshold_qti);     // 
  }
}


void pingSense() {
      long duration, inches, cm;

      pinMode(pingPin, OUTPUT);
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin, LOW);
    
      pinMode(pingPin, INPUT);
      duration = pulseIn(pingPin, HIGH);
      delay(100);
   
      if (duration < threshold_Ping) {
        digitalWrite(46, LOW);
        digitalWrite(45, HIGH);
        tall_block_count++;        
      }
      else {
        digitalWrite(46, HIGH);
        digitalWrite(45, LOW);
      }
      Serial3.println(tall_block_count);
}

//
void goStraight() {
  left.write(122); // left forwards
  right.write(83); // right forwards
}
void stopBot() {
  left.write(93);
  right.write(93);
}

void calculateOutgoing() {
  switch (tall_block_count) {
            case 1:
              outgoing = 'p';
              break;
            case 2:
              outgoing = 'q';
               break;
            case 3:
              outgoing = 'r';
               break;
            case 4:
              outgoing = 's';
               break;
            case 5:
              outgoing = 't';
             break;
            default:
              break;
        }
}
