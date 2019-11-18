#include<SoftwareSerial.h>
#define Rx 17
#define Tx 16
const int buttonPin = 2;
void setup(){
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);

  digitalWrite(46, HIGH);
  digitalWrite(45, HIGH);
}

//Green for sending
//Red for receiving

void loop(){
  if(digitalRead(buttonPin)==HIGH){
    char outgoing = '5';//Serial.read();
    digitalWrite(46, LOW);
    Serial2.write(outgoing);
    delay(500);
    digitalWrite(46, HIGH);
    
  }
  if(Serial2.available()){
    char incoming = Serial2.read();
    if (incoming == '5') {
      Serial.println(incoming);
      digitalWrite(45, LOW);
      delay(500);
      digitalWrite(45, HIGH);
    }
  }
  //delay(50);
}
