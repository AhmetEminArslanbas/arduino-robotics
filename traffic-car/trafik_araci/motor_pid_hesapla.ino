double yaw_pid_i, yaw_last_error;
double yaw_control_signal;

void resetPidVariables() 
{
  yaw_pid_i = 0;
  yaw_last_error = 0;
}

float yaw_pid_hesapla(int hata)
{
  double yawError = hata;


  yaw_control_signal = getControlSignal(yawError, KP_yaw, KI_yaw, KD_yaw, yaw_pid_i, yaw_last_error);
  //Serial.println(yaw_control_signal);
  yaw_control_signal = constrain(yaw_control_signal , -max_toparlama_speed, max_toparlama_speed);
  // motor gücünün mutlağı motor ek powerdan küçük ve 4 derecelik açı varsa durdur; 
  return yaw_control_signal;
}