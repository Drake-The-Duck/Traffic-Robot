#include "UltraSonicDistanceSensor.h"

int motor1Positive = 5;       //2 động cơ bên phải
int motor1Negative = 4;

int motor2Positive = 3;       //2 động cơ bên trái
int motor2Negative = 2;

int callValue;
#define call_1 30
#define call_2 28

int  giatri1 ; //Đọc giá trị cảm biến s1 và gán vào biến giatri1
int  giatri2;
int  giatri3;
int  giatri4;
int  giatri5;
int signal;

// cam bien truoc sau
const int trigBack = 53; //chan trig sau
const int echoBack = 52; // chan echo sau
const int trigFront = 47; // chan trig truoc
const int echoFront = 46; // chan echo truoc

// do line
int s1 = 11; // Cảm biến trái ngoài cùng
int s2 = 10;
int s3 = 9; // Cảm biến giữa
int s4 = 8;
int s5 = 7; // Cảm biến phải ngoài cùng

//sieu am
int leftTrig = 49;
int leftEcho = 48;
int rightTrig = 51;
int rightEcho = 50;

unsigned int distanceLeft, distanceRight;
unsigned int distanceFront, distanceBack;

UltraSonicDistanceSensor distance1(trigBack, echoBack);  // Initialize sensor that uses digital pins 43 and 42
UltraSonicDistanceSensor distance2(trigFront, echoFront);  // Initialize sensor that uses digital pins 45 and 44
double valuecm1 = 0;
double valuecm2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(motor1Positive, OUTPUT);
  pinMode(motor1Negative, OUTPUT);
  pinMode(motor2Positive, OUTPUT);
  pinMode(motor2Negative, OUTPUT);
  pinMode(call_1, INPUT_PULLUP);
  pinMode(call_2, INPUT_PULLUP);


  //do line

  pinMode(s1, INPUT); //Cảm biến nhận tín hiệu
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);

  //sieu am
  pinMode(leftTrig, OUTPUT);
  pinMode(rightTrig, OUTPUT);
  digitalWrite(leftTrig, HIGH);
  digitalWrite(rightTrig, HIGH);
  pinMode(leftEcho, INPUT);
  pinMode(rightEcho, INPUT);
  delay(500);
}


void leftSensor() {
  Serial.print("Distance Left = ");
  digitalWrite(leftTrig, LOW);
  digitalWrite(leftTrig, HIGH);

  unsigned long LowLevelTimeLeft  = pulseIn(leftEcho, LOW);
  if (LowLevelTimeLeft >= 50000) {
    Serial.println("Invalid");
  }
  else
  {
    distanceLeft = LowLevelTimeLeft / 50;
    Serial.print(distanceLeft);
    Serial.println(" cm");
  }
}

void rightSensor() {
  Serial.print("Distance Right = ");
  digitalWrite(rightTrig, LOW);
  digitalWrite(rightTrig, HIGH);

  unsigned long LowLevelTimeRight  = pulseIn(rightEcho, LOW);

  if (LowLevelTimeRight >= 50000) {
    Serial.println("Invalid");
  } else {
    distanceRight = LowLevelTimeRight / 50;
    Serial.print(distanceRight);
    Serial.println(" cm");
  }
}

void frontSensor() {
  if(distance2.available() == true){
        distanceFront = distance2.getcm();
        Serial.println(distanceFront);
    }
}

void backSensor() {
  if(distance1.available() == true)
    {
      distanceBack = distance1.getcm();
      Serial.println(distanceBack);

    }
}

void turnRight() {
  digitalWrite(motor1Positive, HIGH);
  digitalWrite(motor1Negative, LOW);
  digitalWrite(motor2Positive, HIGH);
  digitalWrite(motor2Negative, LOW);
}

void turnLeft() {
  digitalWrite(motor1Positive, LOW);
  digitalWrite(motor1Negative, HIGH);
  digitalWrite(motor2Positive, LOW);
  digitalWrite(motor2Negative, HIGH);
}

void goForward() {
  digitalWrite(motor2Positive, HIGH);
  digitalWrite(motor2Negative, LOW);
  digitalWrite(motor1Positive, LOW);
  digitalWrite(motor1Negative, HIGH);
}

void goBackward() {
  digitalWrite(motor2Positive, LOW);
  digitalWrite(motor2Negative, HIGH);
  digitalWrite(motor1Positive, HIGH);
  digitalWrite(motor1Negative, LOW);
}

void stop() {
  digitalWrite(motor1Positive, LOW);
  digitalWrite(motor1Negative, LOW);
  digitalWrite(motor2Positive, LOW);
  digitalWrite(motor2Negative, LOW);
}

void loop()
{
  int  giatri1 = digitalRead(s1); //Đọc giá trị cảm biến s1 và gán vào biến giatri1
  int  giatri2 = digitalRead(s2);
  int  giatri3 = digitalRead(s3);
  int  giatri4 = digitalRead(s4);
  int  giatri5 = digitalRead(s5);

  //sieu am
  leftSensor();
  rightSensor();
  backSensor();
  frontSensor();
  
  
  //sieu am
  
  Serial.print("S2: ");
  Serial.print(giatri2); Serial.print("   ");
  Serial.print("S3: ");
  Serial.print(giatri3); Serial.print("   ");
  Serial.print("S4: ");
  Serial.print(giatri4); Serial.print("   ");
  Serial.println(digitalRead(call_1));
  Serial.println(digitalRead(call_2));


  
  if ((digitalRead(call_1) == 0) || (digitalRead(call_2) == 0) || (distanceBack <= 40)) {
    if ((giatri2 == 0) && (giatri3 == 0) && (giatri4 == 0)) {
      if ((distanceRight <= 50) || (distanceFront <= 30)) {
        Serial.println("Nhuong xe di qua");
        stop();
      } else if ((distanceRight > 50) && (distanceFront > 30)) {
        Serial.println("Di thang");
        goForward();
      }
    } else if ((giatri2 == 1) && (giatri3 == 1) && (giatri4 == 1)) {
      turnRight();     
      Serial.println("turn around");
    }
  } else if ((digitalRead(call_1) == 1) && (digitalRead(call_2) == 1)) {
    stop();
    Serial.println("stop2");
  }
  delay(500);
}
