
void pid(){
 float duzeltmehizi=Kp*hata+Kd*(hata-sonhata);  
 sonhata=hata;


 sagmotorpwm=(tabanhiz+ekhiz)+duzeltmehizi;
 solmotorpwm=(tabanhiz+ekhiz)-duzeltmehizi;

if(hata >= 3500 || hata <= -3500)
{
  sagmotorpwm=constrain(sagmotorpwm / 5.5,-150,150); 
  solmotorpwm=constrain(solmotorpwm / 5.5, -150,150);
}
else
{
  sagmotorpwm=constrain(sagmotorpwm,-194,194);
  solmotorpwm=constrain(solmotorpwm, -194,194);
}
//motorkontrol(solmotorpwm,sagmotorpwm);
}


double getControlSignal(double error, double kp, double ki, double kd, double& pid_i, double& last_error, double delta_time_in_seconds) 
{
  double pid_p = error;
  double pid_d = (error - last_error) / delta_time_in_seconds;
  pid_i += error * delta_time_in_seconds;

  double control_signal = (kp*pid_p) + (ki*pid_i) + (kd*pid_d);
  last_error = error;
  return control_signal;
}
