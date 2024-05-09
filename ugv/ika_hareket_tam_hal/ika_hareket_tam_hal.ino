#include "sbus.h"

#define frontEngineRpwm 0
#define frontEngineLpwm 2

#define limitSwitchPin 36

#define role1 23

#define kontakRole 22
#define escRole 27

#define gaz 25

#define potPin 14

#define KP_yaw 30.0//2.80
#define KI_yaw 0.0//0.0005
#define KD_yaw 0.0//400.00

bfs::SbusRx sbus_rx(&Serial2, 16, 17, true); 
bfs::SbusData data;

int button1Value = 0;
int button2Value = 0;
int button3Value = 0;
int throttleValue = 0;
int hedefPos = 0;

bool isDonusTime = false;
bool isDonebilir = false;
long donusTime = 0;
long anlikDonusTime = 0;
long acilDurumTime = 0;
int maxDonusTime = 10000;
int durum = 0;

int rollValue = 0;
int rollDeger = 1500;

int gecisBeklemesi = 500;

bool isAcilDurum = false;

bool isRC = false;
bool firstFailsafeRC = false;

double yaw_pid_i, yaw_last_error;

void setup()
{
  Serial.begin(115200);

  sbus_rx.Begin(); 


  pinMode(role1, OUTPUT);
  pinMode(kontakRole, OUTPUT);
  pinMode(escRole, OUTPUT);
  pinMode(gaz, OUTPUT);

  pinMode(limitSwitchPin, INPUT_PULLUP);

  pinMode(potPin, INPUT);

  pinMode(frontEngineRpwm, OUTPUT);
  pinMode(frontEngineLpwm, OUTPUT);


  digitalWrite(frontEngineRpwm, LOW);
  digitalWrite(frontEngineLpwm, LOW);

  digitalWrite(role1, LOW);
  digitalWrite(kontakRole, LOW);
  digitalWrite(escRole, LOW);
}

void loop() 
{
  readRCValue();

  checkRC();
  
  if(isRC)
  {
    buttonController();
    //Serial.println(hedefPos);
    analogWrite(gaz, hedefPos); //gaz kontrolu arm/disarm butonu kontrolu ekle

    frontWheelController();
  }
}

void readRCValue()
{
  if (sbus_rx.Read()) 
  {
    data = sbus_rx.data();
    
    if (data.failsafe)
    {
      isRC = false;
      Serial.println("KUMANDA BAGLANTISI KOPTU! (FAILSAFE AKTIF) - MOTORLARI DURDUR!");
    }
    else 
    {
      if (data.lost_frame) 
      {
        Serial.println("DIKKAT: Sinyal zayıf, paket kayıpları yaşanıyor!");
      }
      
      isRC = true;

      for (int i = 0; i < 16; i++) 
      {
        throttleValue = data.ch[2];
        rollDeger = data.ch[0];
        button1Value = data.ch[4];
        button2Value = data.ch[11];
        button3Value = data.ch[6];

        hedefPos =  map(throttleValue, 306, 1693, 62, 255); //map(pulseIn(rollPin, HIGH), 1061, 1930, -30, 30); 147  1062, 1924, 39, 189
        hedefPos = constrain(hedefPos, 62, 255);//62

        rollValue = map(rollDeger, 309, 1693, 30, -30);
        rollValue = constrain(rollValue, -30, 30);
        rollValue-=7;    
        //Serial.print(data.ch[i]);
        //Serial.print("\t");
      }
      //Serial.println();
    }
  }

  Serial.print(throttleValue);
  Serial.print("   ");
  Serial.print(rollDeger);
  Serial.print("   ");

  Serial.print(button1Value);
  Serial.print("   ");
  Serial.print(button2Value);
  Serial.print("   ");
  Serial.println(button3Value);
}

void buttonController()
{
  if(button3Value < 1500) //ileri geri (button2Value < 1700
    digitalWrite(role1, LOW);
  else
    digitalWrite(role1, HIGH);

  if(button2Value < 1300) //kontak arm disarm
    digitalWrite(kontakRole, LOW);
  else
    digitalWrite(kontakRole, HIGH);

  if(button1Value > 1500) //esc button2Value > 1500)
  {
    Serial.println("acil frenn!!!");
    analogWrite(gaz, 60);
    hedefPos = 60;
    acilDurumTime = millis();
    isAcilDurum = true; 
    while(true)
    {
      readRCValue();
      frontWheelController();

      if(button1Value < 1500 && millis() - acilDurumTime > 2600)
      {
        isAcilDurum = false;
        
        digitalWrite(escRole, LOW);
        delay(500);
        digitalWrite(kontakRole, LOW);

        break;
      }
      

      if(millis() - acilDurumTime < 100)
      {

      }
      else if(millis() - acilDurumTime < 600)
      {
        digitalWrite(kontakRole, HIGH);
      }
      else if(millis() - acilDurumTime < 2600)
      {
        digitalWrite(escRole, HIGH);
      }
      else
      {
        digitalWrite(escRole, HIGH);
      }
    }/*
    delay(100);
    digitalWrite(kontakRole, HIGH);
    delay(500);
    digitalWrite(escRole, HIGH);
    delay(2000);*/
  }
}

void frontWheelController()
{
  int nowPos = map(analogRead(potPin), 940, 2530, 30, -30);
  int rightSpeed = getControlSignal(rollValue - nowPos, KP_yaw, KI_yaw, KD_yaw, yaw_pid_i, yaw_last_error);
  rightSpeed = constrain(rightSpeed, -255, 255);
  nowPos = constrain(nowPos, 30, -30);

  //Serial.print(nowPos);
  //Serial.print("\t");
  //Serial.println(rightSpeed);
  
  if(rightSpeed > 70)
  {
    analogWrite(frontEngineRpwm, abs(rightSpeed));
    analogWrite(frontEngineLpwm, 0);
  }
  else if(rightSpeed < -70 && !digitalRead(limitSwitchPin))
  {
    analogWrite(frontEngineRpwm, 0);
    analogWrite(frontEngineLpwm, abs(rightSpeed));
  }
  else
  {
    analogWrite(frontEngineRpwm, 0);
    analogWrite(frontEngineLpwm, 0);
  }
}


double getControlSignal(double error, double kp, double ki, double kd, double& pid_i, double& last_error) {
  double pid_p = error;
  double pid_d = (error - last_error);
  pid_i += error;
  
  double control_signal = (kp*pid_p) + (ki*pid_i) + (kd*pid_d);
  last_error = error;
  return control_signal;
}


void checkRC()
{
  if(!isRC)
  {
    analogWrite(gaz, 60);
    hedefPos = 60;

    analogWrite(frontEngineRpwm, 0);
    analogWrite(frontEngineLpwm, 0);
    rollValue = 0;
    
    if(firstFailsafeRC == true)
    {
      Serial.println("dondu");

      delay(100);
      digitalWrite(kontakRole, HIGH);
      delay(500);
      digitalWrite(escRole, HIGH);
      delay(500);
      firstFailsafeRC = false;
    }
    digitalWrite(kontakRole, HIGH);
    digitalWrite(escRole, HIGH);
  }
  else if(firstFailsafeRC == false)
  {
    digitalWrite(escRole, LOW);
    delay(500);
    digitalWrite(kontakRole, LOW);

    isRC = true;
    firstFailsafeRC = true;
  }
}