double roll_pid_i, roll_last_error;
double roll_control_signal;

double yaw_pid_i, yaw_last_error;
double yaw_control_signal;

void resetPidVariables() 
{
  roll_pid_i = 0;
  roll_last_error = 0;

  yaw_pid_i = 0;
  yaw_last_error = 0;
}

float yaw_pid_hesapla(float hedef, struct IMU_Values imu_values)
{
  double yawError = hedef - imu_values.CurrentOrientation.YawAngle;

  yawError = sinir_koy(yawError);


  yaw_control_signal = getControlSignal(yawError, gyro_KP_yaw, gyro_KI_yaw, gyro_KD_yaw, yaw_pid_i, yaw_last_error, imu_values.DeltaTimeInSeconds);
  //Serial.println(yaw_control_signal);
  //yaw_control_signal = constrain(yaw_control_signal , -max_toparlama_speed, max_toparlama_speed);
  //yaw_pid_i = constrain(yaw_pid_i, -yaw_i_max_min_speed, yaw_i_max_min_speed);
  // motor gücünün mutlağı motor ek powerdan küçük ve 4 derecelik açı varsa durdur; 
  return yaw_control_signal;
}
float yaw_pid_deger = 0;
int durum = 0;
void gyro_motor_hareket(bool ileri_gidiyor)
{
  /*if((millis() - zaman_gyro) > 200)
  {
    /*switch(durum)
    {
      case 0:
      {
        hedef_aci = 30;
        durum ++;
      }
      case 1:
      {
        hedef_aci = 0;
        durum ++;
      }
      case 2:
      {
        hedef_aci = -30;
        durum = 0;
      }
    }*/
    /*if(durum == 0)
    {
      hedef_aci = 20;
      durum = 2;
    }
    if(durum == 1)
    {
      hedef_aci = 30;
      durum++;
    }
    else if(durum == 2)
    {
      hedef_aci = 0;
      durum++;
    }
    else if(durum == 3)
    {
      hedef_aci = -20;
      durum = 1;
    }
    
    zaman_gyro = millis();
  }*/

  struct IMU_Values imuValues = GetIMU_Values();

  //analogWrite(buzzer, 150);

  yaw_pid_deger = yaw_pid_hesapla(hedef_aci, imuValues);
  if(!ileri_gidiyor)
  {
    if(yaw_pid_deger>0)
      yaw_pid_deger+=32;
    else if(yaw_pid_deger<0)
      yaw_pid_deger-=32;

    motorkontrol(yaw_pid_deger, -yaw_pid_deger);
  }
  else
  {

    motorkontrol(tabanhiz + yaw_pid_deger, tabanhiz + yaw_pid_deger);
  }
  
}

float sinir_koy(float deger)
{
  if (deger > 180)
    deger -= 360;
  else if (deger < -180)
    deger += 360;

  return deger;
}