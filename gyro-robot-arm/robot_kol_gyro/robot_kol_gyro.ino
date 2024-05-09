#include <Simple_MPU6050.h>

Simple_MPU6050 mpu_1(6);
Simple_MPU6050 mpu_2(6);


unsigned char packetBytes[33];


float gyro_1_x, gyro_1_y, gyro_1_z = 0;
float gyro_2_x, gyro_2_y, gyro_2_z = 0;

void setup() 
{
  Serial.begin(115200);
  
  mpu_initialize();
}

void loop() 
{
  mpu_1_veri_cek();
  mpu_2_veri_cek();

  paket_olustur();
  Serial.write(packetBytes, 33);

  /*Serial.print("gyro_1_x: ");
  Serial.print(gyro_1_x);
  Serial.print("\tgyro_1_y: ");
  Serial.print(gyro_1_y);
  Serial.print("\tgyro_1_z: ");
  Serial.print(gyro_1_z);

  Serial.print("\t\tgyro_2_x: ");
  Serial.print(gyro_2_x);
  Serial.print("\tgyro_2_y: ");
  Serial.print(gyro_2_y);
  Serial.print("\tgyro_2_z: ");
  Serial.println(gyro_2_z);*/
  
}
