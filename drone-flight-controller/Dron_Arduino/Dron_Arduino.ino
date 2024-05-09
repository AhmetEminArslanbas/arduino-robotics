#include <avr/wdt.h>

#define PROGRAM_TIMEOUT_IN_MILLISECONDS WDTO_1S // arduino dondu mu kontrol süresi (1 saniye)

#define RECEIVER_COMMUNICATION_TIMEOUT 1000 //kumanda maximum veri gelmeme süresi
#define RECEIVER_DATA_NOT_RECEIVED_TIME_OUT 500 //x8r kablo bağlantısı gelememe süresi
#define IMU_COMMUNICATION_TIMEOUT_IN_MILLISECONDS 100
#define RECEIVER_MIN_VALUE 306  //kumanda minimum değer
#define RECEIVER_MAX_VALUE 1693 //kumanda maximum değer
#define THROTTLE_START_POINT 10  // başlangıç throttle deger
#define THROTTLE_LIMIT_POINT 180 //max throttle deger
#define QUADCOPTER_MAX_TILT_ANGLE 30 //dron maximum alabiliceği açı
#define INTERRUPT_PIN 2 //gyto int pin
#define IMU_COMMUNICATION_TIMEOUT 200

//gyro kalibre kısmı
#define ACCEL_OFFSET_X 126
#define ACCEL_OFFSET_Y 1637
#define ACCEL_OFFSET_Z 414
#define GYRO_OFFSET_X 74
#define GYRO_OFFSET_Y -91
#define GYRO_OFFSET_Z 39

#define MIN_MOTOR_PULSE_WIDTH 1000 //motor minimum değer
#define MAX_MOTOR_PULSE_WIDTH 2000 //motor maximum değer



#define KP_pitch 0.30
#define KI_pitch 0.10
#define KD_pitch 0.10

#define KP_roll 0.30
#define KI_roll 0.10
#define KD_roll 0.10

struct ReceiverCommands {
  int YawAngle;
  int RollAngle;
  int PitchAngle;
  int Throttle;
  int ArmDisarm_switch;
  int Error;
};

struct Orientation {
  double YawAngle;
  double PitchAngle;
  double RollAngle;
};
struct IMU_Values {
  bool Error;
  bool NewDataAvailable;
  double DeltaTimeInSeconds;
  struct Orientation CurrentOrientation;
  struct Orientation PreviousOrientation;
};

struct MotorPowers 
{
  int FrontRightMotorPower;
  int BackRightMotorPower;
  int FrontLeftMotorPower;
  int BackLeftMotorPower;
};



void setup() 
{
  wdt_enable(PROGRAM_TIMEOUT_IN_MILLISECONDS);

  initializeMotor();
  initializeReceiver();
  initializeIMU();
  Serial.begin(115200);
}

void loop() 
{
  wdt_reset();

  kmd_veri_al();
  struct ReceiverCommands receiverCommands = GetReceiverCommands();
  struct IMU_Values imuValues = GetIMUvalues();

  //kumanda veri kkontrol
  Serial.print("   thrt: ");
  Serial.print(receiverCommands.Throttle);
  Serial.print("   yaw: ");
  Serial.print(receiverCommands.YawAngle);
  Serial.print("   pitch: ");
  Serial.print(receiverCommands.PitchAngle);
  Serial.print("   rol: ");
  Serial.print(receiverCommands.RollAngle);
  Serial.print("   arm_swc: ");
  Serial.print(receiverCommands.ArmDisarm_switch);
  Serial.print("   Error: ");
  Serial.print(receiverCommands.Error); //gyro veri okunmicaksa "ln" yap

  //gyro_veri_kontrol //gyro verilerin uçuşa hazır olduğunu kontrol et(yon ve değişken bakımından)
  Serial.print("   gyr_x: ");
  Serial.print(imuValues.CurrentOrientation.PitchAngle);
  Serial.print("   gyr_y: ");
  Serial.print(imuValues.CurrentOrientation.RollAngle);
  Serial.print("   gyr_z: ");
  Serial.println(imuValues.CurrentOrientation.YawAngle);

  if(receiverCommands.Error || imuValues.Error || receiverCommands.Throttle < THROTTLE_START_POINT)// || receiverCommands.ArmDisarm_switch < 1) //arm disarm yeri
  { //bunu "ArmDisarm_switch" butonuna göre yap ve throttle < 20 kalsın (örnekteki gibi)
    stopMotors();
    resetPidVariables();
    return;
  }

  if(imuValues.NewDataAvailable == false){
    return;
  }

  struct MotorPowers motorPowers = calculateMotorPowers(receiverCommands, imuValues);
  spinMotors(motorPowers);
}