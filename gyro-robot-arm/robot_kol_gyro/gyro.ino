void mpu_initialize()
{
  mpu_1.begin();
  mpu_1.SetAddress(0x68);

  mpu_2.begin();
  mpu_2.SetAddress(0x69);

  mpu_1.Set_DMP_Output_Rate_Hz(20);
  mpu_2.Set_DMP_Output_Rate_Hz(20);
  
  mpu_1.CalibrateMPU().Enable_Reload_of_DMP(6).load_DMP_Image();
  mpu_2.CalibrateMPU().Enable_Reload_of_DMP(6).load_DMP_Image();
}

void mpu_1_veri_cek()
{
  Quaternion q;
  VectorFloat gravity;
  float ypr[3] = { 0, 0, 0 };
  float xyz[3] = { 0, 0, 0 };

  mpu_1.GetQuaternion(&q, mpu_1.quat);
  mpu_1.GetGravity(&gravity, &q);
  mpu_1.GetYawPitchRoll(ypr, &q, &gravity);
  mpu_1.ConvertToDegrees(ypr, xyz);

  gyro_1_x = xyz[2];
  gyro_1_y = xyz[1];
  gyro_1_z = xyz[0];

  mpu_1.dmp_read_fifo(0);
}

void mpu_2_veri_cek()
{
  Quaternion q;
  VectorFloat gravity;
  float ypr[3] = { 0, 0, 0 };
  float xyz[3] = { 0, 0, 0 };

  mpu_2.GetQuaternion(&q, mpu_2.quat);
  mpu_2.GetGravity(&gravity, &q);
  mpu_2.GetYawPitchRoll(ypr, &q, &gravity);
  mpu_2.ConvertToDegrees(ypr, xyz);

  gyro_2_x = xyz[2];
  gyro_2_y = xyz[1];
  gyro_2_z = xyz[0];

  mpu_2.dmp_read_fifo(0);
}