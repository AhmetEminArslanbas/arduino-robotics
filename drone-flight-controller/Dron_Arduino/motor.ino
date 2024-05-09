#include <Servo.h>

Servo frontRightMotor;
Servo backRightMotor;
Servo frontLeftMotor;
Servo backLeftMotor;


void initializeMotor()
{
  frontRightMotor.attach(9, MIN_MOTOR_PULSE_WIDTH, MAX_MOTOR_PULSE_WIDTH); //motor yönlerini ayarla
  backRightMotor.attach(10, MIN_MOTOR_PULSE_WIDTH, MAX_MOTOR_PULSE_WIDTH);
  frontLeftMotor.attach(11, MIN_MOTOR_PULSE_WIDTH, MAX_MOTOR_PULSE_WIDTH);
  backLeftMotor.attach(12, MIN_MOTOR_PULSE_WIDTH, MAX_MOTOR_PULSE_WIDTH);
  stopMotors();
  //delay(5000); //çalışmıyosa delayı aç
}

void spinMotors(struct MotorPowers motorPowers)
{
  frontRightMotor.write(motorPowers.FrontRightMotorPower);
  backRightMotor.write(motorPowers.BackRightMotorPower);
  frontLeftMotor.write(motorPowers.FrontLeftMotorPower);
  backLeftMotor.write(motorPowers.BackLeftMotorPower);
}

void stopMotors()
{
  frontRightMotor.write(0);
  backRightMotor.write(0);
  frontLeftMotor.write(0);
  backLeftMotor.write(0);
}