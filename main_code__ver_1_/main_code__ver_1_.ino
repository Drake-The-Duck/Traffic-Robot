#include "UltraSonicDistanceSensor.h"

int motor1Positive = 40;       //2 động cơ bên phải
int motor1Negative = 38;

int motor2Positive = 36;       //2 động cơ bên trái
int motor2Negative = 34;

int callValue;
#define call_1 30             // điều khiển 1
#define call_2 28             // điều khiển 2

//  ĐÂY LÀ MẤY CÁI BIẾN KHOẢNG CÁCH
const int caution = 50;
const int danger = 20;
const int safe = 10;
const int frontStop = 50;

int redLight = 22;
int yellowLight = 24;

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

unsigned int leftDistance, rightDistance, frontDistance, backDistance;

UltraSonicDistanceSensor distanceBack(trigBack, echoBack);  // Initialize sensor that uses digital pins 43 and 42
UltraSonicDistanceSensor distanceFront(trigFront, echoFront);  // Initialize sensor that uses digital pins 45 and 44
double valuecm1 = 0;
double valuecm2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);

  //Động cơ
  
  //EnA & EnB
  pinMode(5, OUTPUT);       //EnB
  pinMode(6, OUTPUT);       //EnA
  
  //Nguồn
  pinMode(motor1Positive, OUTPUT);
  pinMode(motor1Negative, OUTPUT);
  pinMode(motor2Positive, OUTPUT);
  pinMode(motor2Negative, OUTPUT);

  //điều khiển
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
    leftDistance = LowLevelTimeLeft / 50;
    Serial.print(leftDistance);
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
    rightDistance = LowLevelTimeRight / 50;
    Serial.print(rightDistance);
    Serial.println(" cm");
  }
}

void backSensor() {
  if (distanceBack.available() == true) {
    backDistance = distanceBack.getcm();

    Serial.println(backDistance);
  }
}

void frontSensor() {
  if (distanceFront.available() == true)
  {
    frontDistance = distanceFront.getcm();
    Serial.println(frontDistance);

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
  analogWrite(6, 225);        //EnA Pin
  analogWrite(5, 225);        //EnB Pin
  
  digitalWrite(yellowLight, HIGH);
  digitalWrite(redLight, HIGH);

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


  if ((digitalRead(call_1) == 0) || (digitalRead(call_2) == 0) || (backDistance <= 40)) {
    if ((rightDistance > caution) && (leftDistance > caution) && (frontDistance > frontStop)) {
      if ((giatri2 == 0) && (giatri3 == 0) && (giatri4 == 0)) {
        goForward();
        Serial.println("Khong chuong ngai, di thang");
      }
      if ((giatri2 == 1) && (giatri3 <= 1) && (giatri4 == 0)) {
        turnRight();
      }
      if ((giatri2 == 0) && (giatri3 <= 1) && (giatri4 == 1)) {
        turnLeft();
      }
    }
    else if ((danger < rightDistance <= caution) || (danger < leftDistance <= caution)) {
      digitalWrite(yellowLight, LOW);       //bat den vang
//      analogWrite(5, 200);        //giam toc do
//      analogWrite(6, 200);        //giam toc do
      Serial.println("Khoang cach canh bao");
      if ((giatri2 == 0) && (giatri3 == 0) && (giatri4 == 0)) {
        goForward();
      }
      if ((giatri2 == 1) && (giatri3 <= 1) && (giatri4 == 0)) {
        turnRight();
      }
      if ((giatri2 == 0) && (giatri3 <= 1) && (giatri4 == 1)) {
        turnLeft();
      }
    }
    if ((rightDistance <= danger) || (leftDistance <= danger) || (frontDistance <= frontStop)) {
      Serial.println("Nguy hiem, dung lai");
      stop();
      digitalWrite(yellowLight, HIGH);
      digitalWrite(redLight, LOW);        //bat den do
    }
    if ((rightDistance <= safe) || (leftDistance <= safe)) {
      goForward();
    }
  } else if ((digitalRead(call_1) == 1) && (digitalRead(call_2) == 1)) {
    stop();
    Serial.println("stop2");
  }
}