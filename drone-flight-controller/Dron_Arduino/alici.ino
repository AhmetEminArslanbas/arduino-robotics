int x8r_throttle = 3;
int x8r_yaw = 4;
int x8r_pitch = 5;
int x8r_rol = 6;

int x8r_btn_sd = 7;
int x8r_btn_sa = 8;


float throttle = 0;
float yaw = 0;
float pitch = 0;
float rol = 0;

float btn_sd = 1;
float btn_sa = 1;

bool error = false;



int prevRollAngle = 0;
int prevPitchAngle = 0;
int prevThrottle = 0;
int prevYawAngle = 0;
int prevArmDisarm_switch = 0;

double zaman = 0;
float kmd_ilk_veri_toplam = 0;
float kmd_anlik_veri_toplam = 1;

void initializeReceiver()
{
  pinMode(x8r_throttle, INPUT);
  pinMode(x8r_yaw, INPUT);
  pinMode(x8r_pitch, INPUT);
  pinMode(x8r_rol, INPUT);

  pinMode(x8r_btn_sd, INPUT);
}

void kmd_veri_al()
{
  //kumanda veri okuma kısmı
  if(digitalRead(x8r_throttle) == LOW || //x8r kablosu çıktı ise dronu durdur
     digitalRead(x8r_yaw) == LOW || 
     digitalRead(x8r_pitch) == LOW || 
     digitalRead(x8r_rol) == LOW ||
     digitalRead(x8r_btn_sd) == LOW)
  {
    //dronu otomatik indir daha sonradan
    error = true;
  }
  else //x8r bağlantısında sorun yoksa
  {
    throttle = pulseIn(x8r_throttle, HIGH, RECEIVER_DATA_NOT_RECEIVED_TIME_OUT);
    yaw = pulseIn(x8r_yaw, HIGH, RECEIVER_DATA_NOT_RECEIVED_TIME_OUT);
    pitch = pulseIn(x8r_pitch, HIGH, RECEIVER_DATA_NOT_RECEIVED_TIME_OUT);
    rol = pulseIn(x8r_rol, HIGH, RECEIVER_DATA_NOT_RECEIVED_TIME_OUT);

    btn_sd = pulseIn(x8r_btn_sd, HIGH, RECEIVER_DATA_NOT_RECEIVED_TIME_OUT);
    error = false;
  }
}

struct ReceiverCommands GetReceiverCommands()
{
  struct ReceiverCommands cmd;
  /*cmd.RollAngle = prevRollAngle;
  cmd.PitchAngle = prevPitchAngle;
  cmd.Throttle = prevThrottle;
  cmd.YawAngle = prevYawAngle;
  cmd.ArmDisarm_switch = prevArmDisarm_switch;*/
  

  cmd.Throttle = map(throttle, RECEIVER_MIN_VALUE, RECEIVER_MAX_VALUE, 0, THROTTLE_LIMIT_POINT);
  cmd.YawAngle = map(yaw, RECEIVER_MIN_VALUE, RECEIVER_MAX_VALUE, -QUADCOPTER_MAX_TILT_ANGLE, QUADCOPTER_MAX_TILT_ANGLE);
  cmd.PitchAngle = map(pitch, RECEIVER_MIN_VALUE, RECEIVER_MAX_VALUE, -QUADCOPTER_MAX_TILT_ANGLE, QUADCOPTER_MAX_TILT_ANGLE);
  cmd.RollAngle = map(rol, RECEIVER_MIN_VALUE, RECEIVER_MAX_VALUE, -QUADCOPTER_MAX_TILT_ANGLE, QUADCOPTER_MAX_TILT_ANGLE);
  
  cmd.ArmDisarm_switch = map(btn_sd, RECEIVER_MIN_VALUE, RECEIVER_MAX_VALUE, -1, 1);

  cmd.Error = error;

  /*prevRollAngle = cmd.RollAngle;
  prevPitchAngle = cmd.PitchAngle;
  prevThrottle = cmd.Throttle;
  prevYawAngle = cmd.YawAngle;
  prevArmDisarm_switch = cmd.ArmDisarm_switch;*/



  /*if(millis() - zaman > RECEIVER_COMMUNICATION_TIMEOUT) //kumanda girmesi durumunda dronu durdur   //bunu sonradan aç ve dene
  {
    kmd_anlik_veri_toplam = cmd.Throttle + cmd.YawAngle + cmd.PitchAngle + cmd.RollAngle;
    if(kmd_ilk_veri_toplam == kmd_anlik_veri_toplam)
    {
      cmd.Error = true;
    }
    else
    {
      cmd.Error = false;
    }
    kmd_ilk_veri_toplam = kmd_anlik_veri_toplam;
    zaman = millis();
  }*/

  return cmd;
}