#include <Servo.h>

Servo left;
Servo right;

int QTIpins[] = {49, 51, 53}; // left, center, right
int QTIdurations[3];
int QTIvalues[3];

int threshold = 100;
int count = 0;

void setup() {
  left.attach(11);
  right.attach(12);
  Serial.begin(9600);
}

void loop() {  
  // if left black, turn left
  // if right black, turn right
  // if all three, increment hashmarks

  checkSensors();

  if (QTIvalues[0] and not QTIvalues[2]) { // if left, turn right
    left.write(90); // left backwards
    right.write(90); // right forwards
  }
  else if (QTIvalues[2] and not QTIvalues[0]) { // if right, turn left
    left.write(96); // left forwards
    right.write(96); // right backwards
  }
  else { //otherwise to straight
    if(QTIvalues[0] and QTIvalues[1] and QTIvalues[2]) {
      count++;
      Serial.println(count);
      left.write(93);
      right.write(93);
      delay(500);

      if(count == 5){
        left.write(93);
        right.write(93);
        delay(5000000000000);
      }

      left.write(122);
      right.write(82);
      delay(200);
    }
    left.write(122);
    right.write(82);
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
    
    QTIvalues[i] = (duration > threshold);     // converts to Boolean
  }
}
