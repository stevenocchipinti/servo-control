// https://www.xsimulator.net/community/threads/data-from-simtool-to-arduino.8459/#post-105546
#include <Servo.h>
#include <math.h>

String inString = ""; // string to hold input
int Axis1 = 0; // integer to hold axis1
int Axis2 = 0; // integer to hold axis2

const int LEDRED = D8;
const int LEDGREEN = D7;
const int LEDBLUE = D6;

const int LEFT = D1;
const int RIGHT = D2;

Servo leftServo;
Servo rightServo;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);

  Serial.println("Waiting for serial");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  leftServo.attach(LEFT);
  rightServo.attach(RIGHT);

  digitalWrite(LEDRED, HIGH);
  delay(100);
  digitalWrite(LEDRED, LOW);
  digitalWrite(LEDBLUE, HIGH);
  delay(100);
  digitalWrite(LEDBLUE, LOW);
  digitalWrite(LEDGREEN, HIGH);
  delay(100);
  digitalWrite(LEDGREEN, LOW);
}


int byteToDegrees(int byteInt) {
  int rel = byteInt - 127;
  if (rel < 0) return round(90 - ((-rel / 127.0) * 90));
  if (rel == 0) return 90;
  if (rel > 0) return round(90 + ((rel / 128.0) * 90));
}


void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();

    if (isDigit(inChar)) {
      inString += (char)inChar;
    }

    if (inChar == 'a') {
      Axis1 = (inString.toInt());
      leftServo.write(byteToDegrees(Axis1));
      inString = "";
    }

    if (inChar == 'b') {
      Axis2 = (inString.toInt());
      rightServo.write(byteToDegrees(Axis2));
      inString = "";
    }
  }
}
