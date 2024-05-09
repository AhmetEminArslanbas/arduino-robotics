double pitch_pid_i, pitch_last_error;
double pitch_control_signal;

double roll_pid_i, roll_last_error;
double roll_control_signal;



void resetPidVariables() 
{
  pitch_pid_i = 0;
  pitch_last_error = 0;

  roll_pid_i = 0;
  roll_last_error = 0;
}

struct MotorPowers calculateMotorPowers(struct ReceiverCommands receiverCommands, struct IMU_Values imu_values) 
{
  double pitchError = receiverCommands.PitchAngle - imu_values.CurrentOrientation.PitchAngle;
  double rollError = receiverCommands.RollAngle - imu_values.CurrentOrientation.RollAngle;

  pitch_control_signal = getControlSignal(pitchError, KP_pitch, KI_pitch, KD_pitch, pitch_pid_i, pitch_last_error, imu_values.DeltaTimeInSeconds);
  roll_control_signal = getControlSignal(rollError, KP_roll, KI_roll, KD_roll, roll_pid_i, roll_last_error, imu_values.DeltaTimeInSeconds);
  

  
  struct MotorPowers motorPowers;
  /*motorPowers.FrontRightMotorPower = receiverCommands.Throttle - pitch_control_signal - roll_control_signal;
  motorPowers.BackRightMotorPower =  receiverCommands.Throttle + pitch_control_signal - roll_control_signal;
  motorPowers.FrontLeftMotorPower =  receiverCommands.Throttle - pitch_control_signal + roll_control_signal;
  motorPowers.BackLeftMotorPower =   receiverCommands.Throttle + pitch_control_signal + roll_control_signal;*/

  motorPowers.FrontRightMotorPower = receiverCommands.Throttle - pitch_control_signal;//geçici
  motorPowers.BackRightMotorPower =  receiverCommands.Throttle + pitch_control_signal;//geçici
  motorPowers.FrontLeftMotorPower =  receiverCommands.Throttle - pitch_control_signal;//geçici
  motorPowers.BackLeftMotorPower =   receiverCommands.Throttle + pitch_control_signal;//geçici
  return motorPowers;
}