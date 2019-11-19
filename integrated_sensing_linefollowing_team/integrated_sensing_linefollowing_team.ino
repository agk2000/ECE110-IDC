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
int threshold = 100;
int count = 0;
const int pingPin = 7;
int tall_block_count = 0;

//Communication
#include<SoftwareSerial.h>
#define Rx 17
#define Tx 16

void setup() {
  //Line sensing
  left.attach(11);
  right.attach(12);
  Serial.begin(9600);

  //Sensing
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);

  //LCD
  Serial3.begin(9600);
  Serial3.println("                    ");

  //Communication
  Serial2.begin(9600);

  //turn off on-board LED
  digitalWrite(46, HIGH);
  digitalWrite(45, HIGH);
}

void loop() {
  // if left black, turn left
  // if right black, turn right
  // if all three, increment hashmarks

  checkSensors();

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
      Serial.println(count);

      left.write(93);
      right.write(93);

//********************************* 
      long duration, inches, cm;

      pinMode(pingPin, OUTPUT);
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin, LOW);
    
      pinMode(pingPin, INPUT);
      duration = pulseIn(pingPin, HIGH);
      //Serial.println(duration);
      delay(100);
    
      long threshold = 200;
    
      if (duration < threshold) {
        //digitalWrite(8, HIGH);
        digitalWrite(46, LOW);
        digitalWrite(45, HIGH);
        tall_block_count++;

        //Serial2.write('t');
        
      }
      else {
        //digitalWrite(8, LOW);
        digitalWrite(46, HIGH);
        digitalWrite(45, LOW);
        //delay(100);
        //Serial2.write('s');
        //delay(100);
      }
      Serial3.println(tall_block_count);

      
//**************************************
      delay(500);

      if(count == 5){
        left.write(93);
        right.write(93);
        left.detach();
        right.detach();

        Serial2.print(tall_block_count);
        Serial2.println();
        
        for(int i = 0;  i < 10; i++) {
          switch (tall_block_count) {
            case 1:
              Serial2.write('p');
              break;
            case 2:
              Serial2.write('q');
               break;
            case 3:
              Serial2.write('r');
               break;
            case 4:
              Serial2.write('s');
               break;
            case 5:
              Serial2.write('t');
             break;
            default:
              break;
            }            
        }
        teamResults[3] = tall_block_count;


        //Team code
        while(!receiveCharacter()) {}
        compute();
        if(minIndex == 0) {Serial3.write("1 is lowest.");}
        if(minIndex == 1) {Serial3.write("2 is lowest.");}
        if(minIndex == 2) {Serial3.write("3 is lowest.");}
        if(minIndex == 3) {Serial3.write("4 is lowest.");}
        while(!receiveFinalRoutine()) {}
        if(finalRoutine == 'x') {Serial3.write("FRX");}
        if(finalRoutine == 'y') {Serial3.write("FRY");}
        if(finalRoutine == 'z') {Serial3.write("FRZ");}
        delay(50000000000);
        
      }
      left.write(122);
      right.write(82);
      delay(200);
    }
    left.write(122);
    right.write(82);
  }
}

void sendCharacter(char c) {
  char outgoing = c;
  Serial2.write(outgoing);
  digitalWrite(45, LOW);
  delay(50);
}

//Team Code
boolean receiveFinalRoutine() {
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
    if(teamResults[i] < teamResults[minIndex]) {
      minIndex = i;
    }
  }
}

/* 
 * INPUT:  Reads the QTIvalues from the QTI sensors stored in the array QTIpins[]
 * OUTPUT: A 0 or 1 in array QTIvalues[] depending on if the sensor reads a dark or light objects
 * In order to calibrate what is dark and what is light, use the threshold variable above
 * 
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
    QTIvalues[i] = (duration > threshold);     // 
  }
}
