#include <Servo.h>

const int Serial_BAUDRATE = 9600;

//腳位宣告-start----------------------------------------------------------------
//需調整-------------------------------
const int Joystick_switch_PIN = 7;  //|
const int Joystick_X_PIN = 14;      //|
const int Joystick_Y_PIN = 15;      //|
const int Motor_A_PIN_1 = 12;       //|
const int Motor_A_PIN_2 = 11;       //|
const int Servo_A_PIN = 10;         //|
const int Servo_B_PIN = 9;          //|
const int Servo_C_PIN = 8;          //|
//需調整-------------------------------
//腳位宣告-end----------------------------------------------------------------

//伺服馬達-start----------------------------------------------------------------
// Servo_A->推球
// Servo_B->俯仰
// Servo_C->水平
Servo Servo_A, Servo_B, Servo_C;
//需調整-------------------------------
const int Servo_A_Angle_MAX = 0;    //|
const int Servo_A_Angle_MIN = 105;   //|
//需調整-------------------------------
int Servo_B_angle = 30;
int Servo_C_speed = 0;
//伺服馬達-end----------------------------------------------------------------

//射球設定-start----------------------------------------------------------------
//可調整-------------------------------
const int shot_timeout = 2000;      //|
//可調整-------------------------------
long last_press_time = 0;
boolean shotting = false;
//射球設定-end----------------------------------------------------------------

int Joystick_X = 0;
int Joystick_Y = 0;

void setup() {
  Serial.begin(Serial_BAUDRATE);

  //腳位設定-start----------------------------------------------------------------
  pinMode(Joystick_switch_PIN, INPUT);
  pinMode(Joystick_X_PIN, INPUT);
  pinMode(Joystick_Y_PIN, INPUT);
  pinMode(Motor_A_PIN_1, OUTPUT);
  pinMode(Motor_A_PIN_2, OUTPUT);
  pinMode(Servo_A_PIN, OUTPUT);
  pinMode(Servo_B_PIN, OUTPUT);
  pinMode(Servo_C_PIN, OUTPUT);
  //腳位設定-end----------------------------------------------------------------

  //伺服馬達設定-start----------------------------------------------------------------
  Servo_A.attach(Servo_A_PIN);
  Servo_B.attach(Servo_B_PIN);
  Servo_C.attach(Servo_C_PIN);
  //伺服馬達設定-end----------------------------------------------------------------
}

void loop() {
  //射球控制-start----------------------------------------------------------------
  Serial.print(digitalRead(Joystick_switch_PIN));
  if (shotting == false) {
    if (millis() - last_press_time >= shot_timeout) {
      if (digitalRead(Joystick_switch_PIN) == HIGH) {
        shotting = true;
        Serial.print("shotting true");
        last_press_time = millis();
      } else {
        shotting = false;
      }
    }
  }
  if (shotting) {
    if ((millis() - last_press_time) >= shot_timeout) {
      shotting = false;
    } else if ((millis() - last_press_time) >= ((shot_timeout / 3) * 2)) {
      Servo_A.write(Servo_A_Angle_MIN);
    } else if ((millis() - last_press_time) >= (shot_timeout / 3)) {
      Servo_A.write(Servo_A_Angle_MAX);
    } else {
      //開啟馬達-start----------------------------------------------------------------
      //需調整-------------------------------
      digitalWrite(Motor_A_PIN_1, HIGH);  //|
      digitalWrite(Motor_A_PIN_2, LOW);   //|
      //需調整-------------------------------
      //開啟馬達-end----------------------------------------------------------------
    }
  } else {
    //關閉馬達-start----------------------------------------------------------------
    digitalWrite(Motor_A_PIN_1, LOW);
    digitalWrite(Motor_A_PIN_2, LOW);
    //關閉馬達-end----------------------------------------------------------------
    //將射球伺服馬達轉動至初始位置
    Servo_A.write(Servo_A_Angle_MIN);
    //Serial.print("Servo_A_Angle_MIN");
  }
  //射球控制-end----------------------------------------------------------------

  //俯仰控制-start----------------------------------------------------------------
  Joystick_X = analogRead(Joystick_X_PIN);
  //Serial.print("X");
  //Serial.print(Joystick_X);

  if (Joystick_X > 500) {
    Servo_B_angle += 1;
  } else if (Joystick_X < 200) {
    Servo_B_angle -= 1;
  }
  //Serial.print("Servo_B_angle");
  //Serial.print(Servo_B_angle);
  if (Servo_B_angle > 30) {
    Servo_B_angle = 30;
  } else if (Servo_B_angle < 0) {
    Servo_B_angle = 0;
  }
  Servo_B.write(Servo_B_angle);
  //俯仰控制-end----------------------------------------------------------------

  //水平控制-start----------------------------------------------------------------
  Joystick_Y = analogRead(Joystick_Y_PIN);
  //Serial.print("Y");
  //Serial.println(Joystick_Y);
  if (Joystick_Y > 500) {
    Servo_C_speed = map(Joystick_Y, 500, 700, 90, 135);
  } else if (Joystick_Y < 200) {
    Servo_C_speed = map(Joystick_Y, 0, 200, 45, 90);
  }
  Servo_C.write(Servo_C_speed);
  //水平控制-end----------------------------------------------------------------
}
