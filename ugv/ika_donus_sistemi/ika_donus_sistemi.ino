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

#define potPin A0

#include <Servo.h>


#define KP_yaw 2.0//2.80
#define KI_yaw 0//0.0005
#define KD_yaw 0.8//400.00


Servo yawServo;

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

  Serial.println("kalibre ediliyor");

  pinMode(potPin, INPUT);

  yawServo.attach(11);

  initializeIMU(&mpu1, INTERRUPT_1_PIN, 1);
  initializeIMU(&mpu2, INTERRUPT_2_PIN, 2);

  
  
  Serial.println("offset ayari basladi Hareket ettirmeyin!");

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

    if(millis() - setOffsetTime < 1000)
    {
      if(yawOffset != gyro1Yaw - gyro2Yaw)
      {
        setOffset = false;
      }

      Serial.print("Sabit kalması bekleniyor:   ");
      Serial.print(gyro1Yaw - gyro2Yaw);
      Serial.print("   pot: ");
      Serial.println(map(analogRead(potPin), 0, 1023, -30, 30));
    }
    else
    {
      break;
    }
  }

  yawOffset = gyro1Yaw - gyro2Yaw;

  Serial.println("Servo donuse basladi");
}

int yawValue = 0;
int hedefPos = 0;

void loop() 
{
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
  Serial.println((gyro1Yaw-gyro2Yaw) - yawOffset);*/

  yawValue = (gyro1Yaw-gyro2Yaw) - yawOffset;
  
  hedefPos = map(analogRead(potPin), 0, 1023, -30, 30);


  yaw_control_signal = getControlSignal(yawValue - hedefPos, KP_yaw, KI_yaw, KD_yaw, yaw_pid_i, yaw_last_error, (imu1.DeltaTime + imu2.DeltaTime) / 2);

  Serial.print(yaw_control_signal);
  Serial.print("   hedef: ");
  Serial.print(hedefPos);
  Serial.print("   anlik: ");
  Serial.println(yawValue);

  if(abs(yawValue) > 90)
  {
    yawServo.write(90);
    //Serial.println("sinirler asildi servo durduruldu");
    //yawServo.write(yaw_control_signal + 90);
  }
  else
  {
    yawServo.write(yaw_control_signal + 90);
  }
}
