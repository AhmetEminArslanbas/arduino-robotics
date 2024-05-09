double getControlSignal(double error, double kp, double ki, double kd, double& pid_i, double& last_error) 
{
  double pid_p = error;
  double pid_d = (error - last_error);
  pid_i += error;

  double control_signal = (kp*pid_p) + (ki*pid_i) + (kd*pid_d);
  last_error = error;
  return control_signal;
}