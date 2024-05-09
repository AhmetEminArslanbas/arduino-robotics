#define motorRole1 8
#define motorRole2 9
#define motorRole3 11
#define motorRole4 10

#define throttlePin 5
#define rollPin 4


#define INTERRUPT_1_PIN 3 //gyto int pin
#define INTERRUPT_2_PIN 2
#define IMU_COMMUNICATION_TIMEOUT 200

//gyro kalibre kısmı
#define ACCEL1_OFFSET_X 5602
#define ACCEL1_OFFSET_Y -7064
#define ACCEL1_OFFSET_Z 9178
#define GYRO1_OFFSET_X 3
#define GYRO1_OFFSET_Y -45
#define GYRO1_OFFSET_Z -55

#define ACCEL2_OFFSET_X 842
#define ACCEL2_OFFSET_Y -6375
#define ACCEL2_OFFSET_Z 1682
#define GYRO2_OFFSET_X -1198
#define GYRO2_OFFSET_Y -800
#define GYRO2_OFFSET_Z 44


bool isDonusTime = false;
bool isDonebilir = false;
long donusTime = 0;
long anlikDonusTime = 0;
int maxDonusTime = 2000;
int durum = 0;

int rollValue = 1500;

int gecisBeklemesi = 500;

#define KP_yaw 0.2//2.80
#define KI_yaw 0//0.0005
#define KD_yaw 0.0//400.00


struct Orientation {
  double YawAngle;
  double PitchAngle;
  double RollAngle;
};

struct IMU_Values {
  bool Error;
  bool NewDataAvailable;
  unsigned long DeltaTime;
  struct Orientation CurrentOrientation;
};

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu1(0x68);
MPU6050 mpu2(0x69);

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

unsigned long last_time = 0; 

int yawOffset = 0;
int gyro1Yaw;
int gyro2Yaw;

bool setOffset = false;
long setOffsetTime = 0;

double yaw_pid_i, yaw_last_error;
double yaw_control_signal;

int yawValue = 0;
int hedefPos = 0;

void initializeIMU(MPU6050* mpu_, uint8_t INTERRUPT_PIN, int number)
{
    Wire.begin();
    Wire.setClock(400000);

    mpu_->initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    if (!mpu_->testConnection()) {
        Serial.println("*imu test connection failed!");
    }

    devStatus = mpu_->dmpInitialize();

    if(number == 1)
    {
      mpu_->setXGyroOffset(GYRO1_OFFSET_X);
      mpu_->setYGyroOffset(GYRO1_OFFSET_Y);
      mpu_->setZGyroOffset(GYRO1_OFFSET_Z);

      mpu_->setXAccelOffset(ACCEL1_OFFSET_X);
      mpu_->setYAccelOffset(ACCEL1_OFFSET_Y);
      mpu_->setZAccelOffset(ACCEL1_OFFSET_Z);
    }
    else if(number == 2)
    {
      mpu_->setXGyroOffset(GYRO2_OFFSET_X);
      mpu_->setYGyroOffset(GYRO2_OFFSET_Y);
      mpu_->setZGyroOffset(GYRO2_OFFSET_Z);

      mpu_->setXAccelOffset(ACCEL2_OFFSET_X);
      mpu_->setYAccelOffset(ACCEL2_OFFSET_Y);
      mpu_->setZAccelOffset(ACCEL2_OFFSET_Z);
    }

    

    if (devStatus == 0) {
        mpu_->setDMPEnabled(true);

        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

        mpuIntStatus = mpu_->getIntStatus();
        dmpReady = true;
        packetSize = mpu_->dmpGetFIFOPacketSize();
    }
}

struct IMU_Values GetIMU_Values(MPU6050* mpu_)
{
    struct IMU_Values o;
    o.NewDataAvailable = false;

    if (!dmpReady)
        return o;

    unsigned long current_time = millis();
    unsigned long delta_time = current_time - last_time;

    if (mpu_->dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu_->dmpGetQuaternion(&q, fifoBuffer);
        mpu_->dmpGetGravity(&gravity, &q);
        mpu_->dmpGetYawPitchRoll(ypr, &q, &gravity);

        o.CurrentOrientation.YawAngle = ypr[0] * 180.0 / M_PI;
        o.CurrentOrientation.RollAngle = -ypr[1] * 180.0 / M_PI;
        o.CurrentOrientation.PitchAngle = ypr[2] * 180.0 / M_PI;

        o.NewDataAvailable = true;
        o.DeltaTime = delta_time;

        if (last_time == 0) {
            last_time = current_time;
            o.Error = true;
            return o;
        }

        last_time = current_time;
    }

    if (delta_time > IMU_COMMUNICATION_TIMEOUT)
        o.Error = true;
    else
        o.Error = false;

    return o;
}

double getControlSignal(double error, double kp, double ki, double kd, double& pid_i, double& last_error, unsigned long delta_time) {
  double pid_p = error;
  double pid_d = (error - last_error) / delta_time;
  pid_i += error * delta_time;
  
  double control_signal = (kp*pid_p) + (ki*pid_i) + (kd*pid_d);
  last_error = error;
  return control_signal;
}


void setup() 
{
  Serial.begin(115200);

  pinMode(motorRole1, OUTPUT);
  pinMode(motorRole2, OUTPUT);
  pinMode(motorRole3, OUTPUT);
  pinMode(motorRole4, OUTPUT);

  pinMode(rollPin, INPUT);

  digitalWrite(motorRole1, HIGH);
  digitalWrite(motorRole2, HIGH);
  digitalWrite(motorRole3, HIGH);
  digitalWrite(motorRole4, HIGH);

  initializeIMU(&mpu1, INTERRUPT_1_PIN, 1);
  initializeIMU(&mpu2, INTERRUPT_2_PIN, 2);

  Serial.println("offset ayari basladi Hareket ettirmeyin!");

  delay(2000);

  digitalWrite(motorRole1, LOW);
  digitalWrite(motorRole2, HIGH);
  digitalWrite(motorRole3, HIGH);
  digitalWrite(motorRole4, LOW); 

  delay(5000);

  while(true)
  {
    IMU_Values imu1 = GetIMU_Values(&mpu1);
    IMU_Values imu2 = GetIMU_Values(&mpu2);

    gyro1Yaw = imu1.CurrentOrientation.YawAngle;
    gyro2Yaw = imu2.CurrentOrientation.YawAngle;

    
    if(!setOffset)
    {
      setOffset = true;
      setOffsetTime = millis();

      yawOffset = gyro1Yaw - gyro2Yaw;
    }

    if(millis() - setOffsetTime < 10000)
    {
      if(yawOffset != gyro1Yaw - gyro2Yaw)
      {
        setOffset = false;
      }

      Serial.print("Sabit kalması bekleniyor:   ");
      Serial.print(gyro1Yaw - gyro2Yaw);
      Serial.print("   rollValue: ");
      Serial.println(map(pulseIn(rollPin, HIGH), 1057, 1915, -30, 30));
    }
    else
    {
      break;
    }
  }

  


  yawOffset = gyro1Yaw - gyro2Yaw - 25;

  Serial.println("Servo donuse basladi");
}
bool isSagDonusTime = false;
bool isSolDonusTime = false;

void loop() 
{
  rollValue = pulseIn(rollPin, HIGH);
  hedefPos =  map(rollValue, 1057, 1915, 30, -30); //map(pulseIn(rollPin, HIGH), 1061, 1930, -30, 30);
  hedefPos = constrain(hedefPos, -30, 30);

  IMU_Values imu1 = GetIMU_Values(&mpu1);
  IMU_Values imu2 = GetIMU_Values(&mpu2);

  int gyro1Pitch = imu1.CurrentOrientation.PitchAngle;
  int gyro1Roll = imu1.CurrentOrientation.RollAngle;
  int gyro1Yaw = imu1.CurrentOrientation.YawAngle;

  int gyro2Pitch = imu2.CurrentOrientation.PitchAngle;
  int gyro2Roll = imu2.CurrentOrientation.RollAngle;
  int gyro2Yaw = imu2.CurrentOrientation.YawAngle;
  
  /*Serial.print("   gyr1_x: ");
  Serial.print(gyro1Pitch);
  Serial.print("   gyr1_y: ");
  Serial.print(gyro1Roll);
  Serial.print("   gyr1_z: ");
  Serial.print(gyro1Yaw);

  Serial.print("         gyr2_x: ");
  Serial.print(gyro2Pitch);
  Serial.print("   gyr2_y: ");
  Serial.print(gyro2Roll);
  Serial.print("   gyr2_z: ");
  Serial.print(gyro2Yaw);

  Serial.print("         gyr_fark_x: ");
  Serial.print(gyro1Pitch-gyro2Pitch);
  Serial.print("   gyr_fark_y: ");
  Serial.print(gyro1Roll-gyro2Roll);
  Serial.print("   gyr_fark_z: ");
  Serial.print((gyro1Yaw-gyro2Yaw) - yawOffset);*/

  yawValue = (gyro1Yaw-gyro2Yaw) - yawOffset;

  //yaw_control_signal = getControlSignal(yawValue - hedefPos, KP_yaw, KI_yaw, KD_yaw, yaw_pid_i, yaw_last_error, (imu1.DeltaTime + imu2.DeltaTime) / 2);

  Serial.print("   ");
  Serial.print(yaw_control_signal);
  Serial.print("   hedef: ");
  Serial.print(hedefPos);
  Serial.print("   anlik: ");
  Serial.println(yawValue);

  if(yawValue - hedefPos > 1)
  {
    if(millis() - donusTime < gecisBeklemesi)
    {
      return;
    }
    durum = 1;
    donusTime = millis();

    digitalWrite(motorRole1, HIGH);
    digitalWrite(motorRole2, LOW);
    digitalWrite(motorRole3, LOW);
    digitalWrite(motorRole4, HIGH); 
  }
  else if(yawValue - hedefPos < -1)
  {
    if(millis() - donusTime < gecisBeklemesi)
    {
      return;
    }
    durum = -1;
    donusTime = millis();

    digitalWrite(motorRole1, LOW);
    digitalWrite(motorRole2, HIGH);
    digitalWrite(motorRole3, HIGH);
    digitalWrite(motorRole4, LOW);
  }
  else
  {
    digitalWrite(motorRole1, HIGH);
    digitalWrite(motorRole2, HIGH);
    digitalWrite(motorRole3, HIGH);
    digitalWrite(motorRole4, HIGH);
  }


return;
  if(abs(yawValue) > 30)
  {
    //durdur
    Serial.println("sinirler asildi servo durduruldu");
  }
  else
  {
    //deger uygula

    if(yaw_control_signal > 5)
    {
      if((millis() - donusTime < maxDonusTime))
      {
        if(!isDonusTime)
        {
          if((millis() - anlikDonusTime < gecisBeklemesi) && durum == -1)
          {
            Serial.println("gecis bekleniyor");
            return;
          }

          donusTime = millis();
          isDonusTime = true;
          durum = 1;
        }

        digitalWrite(motorRole1, HIGH);
        digitalWrite(motorRole2, LOW);
        digitalWrite(motorRole3, LOW);
        digitalWrite(motorRole4, HIGH);

        anlikDonusTime = millis();

        Serial.print("Saga Donuyor   ");
      }
      else
      {
        digitalWrite(motorRole1, HIGH);
        digitalWrite(motorRole2, HIGH);
        digitalWrite(motorRole3, HIGH);
        digitalWrite(motorRole4, HIGH);

        Serial.print("Saga Korumaya Girdi   ");
      }
    }
    else if(yaw_control_signal < -5)
    {
      if(millis() - donusTime < maxDonusTime)
      {
        if(!isDonusTime)
        {
          if((millis() - anlikDonusTime < gecisBeklemesi) && durum == 1)
          {
            Serial.println("gecis bekleniyor");
            return;
          }

          donusTime = millis();
          isDonusTime = true;
          durum = -1;
        }

        digitalWrite(motorRole1, LOW);
        digitalWrite(motorRole2, HIGH);
        digitalWrite(motorRole3, HIGH);
        digitalWrite(motorRole4, LOW);

        anlikDonusTime = millis();

        Serial.print("Sola Donuyor   ");
      }
      else
      {
        digitalWrite(motorRole1, HIGH);
        digitalWrite(motorRole2, HIGH);
        digitalWrite(motorRole3, HIGH);
        digitalWrite(motorRole4, HIGH);

        Serial.print("Sola Korumaya Girdi   ");
      }
    }
    else
    {
      digitalWrite(motorRole1, HIGH);
      digitalWrite(motorRole2, HIGH);
      digitalWrite(motorRole3, HIGH);
      digitalWrite(motorRole4, HIGH);

      donusTime = millis();
      isDonusTime = false;

      Serial.print("Donmuyor   ");
    }
  }

  return;





  /*if(millis() - timee < 1000)
  {
    rollValue = 30;
  }
  else if(millis() - timee < 2000)
  {
    rollValue = -30;
  }
  else
  {
    rollValue = 0;
    timee = millis();
  }*/


  if(rollValue > 20)
  {
    if((millis() - donusTime < maxDonusTime))
    {
      if(!isDonusTime)
      {
        if((millis() - anlikDonusTime < gecisBeklemesi) && durum == -1)
        {
          Serial.println("gecis bekleniyor");
          return;
        }

        donusTime = millis();
        isDonusTime = true;
        durum = 1;
      }

      digitalWrite(motorRole1, HIGH);
      digitalWrite(motorRole2, LOW);
      digitalWrite(motorRole3, LOW);
      digitalWrite(motorRole4, HIGH);

      anlikDonusTime = millis();

      Serial.print("Saga Donuyor   ");
    }
    else
    {
      digitalWrite(motorRole1, HIGH);
      digitalWrite(motorRole2, HIGH);
      digitalWrite(motorRole3, HIGH);
      digitalWrite(motorRole4, HIGH);

      Serial.print("Saga Korumaya Girdi   ");
    }
  }
  else if(rollValue < -20)
  {
    if(millis() - donusTime < maxDonusTime)
    {
      if(!isDonusTime)
      {
        if((millis() - anlikDonusTime < gecisBeklemesi) && durum == 1)
        {
          Serial.println("gecis bekleniyor");
          return;
        }

        donusTime = millis();
        isDonusTime = true;
        durum = -1;
      }

      digitalWrite(motorRole1, LOW);
      digitalWrite(motorRole2, HIGH);
      digitalWrite(motorRole3, HIGH);
      digitalWrite(motorRole4, LOW);

      anlikDonusTime = millis();

      Serial.print("Sola Donuyor   ");
    }
    else
    {
      digitalWrite(motorRole1, HIGH);
      digitalWrite(motorRole2, HIGH);
      digitalWrite(motorRole3, HIGH);
      digitalWrite(motorRole4, HIGH);

      Serial.print("Sola Korumaya Girdi   ");
    }
  }
  else
  {
    digitalWrite(motorRole1, HIGH);
    digitalWrite(motorRole2, HIGH);
    digitalWrite(motorRole3, HIGH);
    digitalWrite(motorRole4, HIGH);

    donusTime = millis();
    isDonusTime = false;

    Serial.print("Donmuyor   ");
  }

  Serial.println(rollValue);
}
