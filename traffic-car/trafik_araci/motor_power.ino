void motor_hareket(float left_speed, float right_speed)
{
  right_speed = constrain(right_speed, -255, 255);
  if(right_speed > 0)
  {
    sag_motor.run(BACKWARD);
    sag_motor.setSpeed(right_speed);
  }
  else if(right_speed < 0)
  {
    sag_motor.run(FORWARD);
    sag_motor.setSpeed(-right_speed);
  }
  else
  {
    sag_motor.run(FORWARD);
    sag_motor.setSpeed(0);
  }



  left_speed = constrain(left_speed, -255, 255);
  if(left_speed > 0)
  {
    sol_motor.run(BACKWARD);
    sol_motor.setSpeed(left_speed);
  }
  else if(left_speed < 0)
  {
    sol_motor.run(FORWARD);
    sol_motor.setSpeed(-left_speed);
  }
  else
  {
    sol_motor.run(FORWARD);
    sol_motor.setSpeed(0);
  }
}