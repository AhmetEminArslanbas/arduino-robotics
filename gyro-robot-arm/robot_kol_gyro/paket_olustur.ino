unsigned char check_sum_hesapla(int index){
    int check_sum = 0;
    
    for(int i = 4; i < index; i++){
        check_sum += packetBytes[i];
    }
    
    return (unsigned char) (check_sum % 256); 
}

void paket_olustur()
{
  int index = 0;
  index = 0;


  packetBytes[0] = 0xFF; // Sabit
  index++;
  packetBytes[1] = 0xFF; // Sabit
  index++;
  packetBytes[2] = 0x54; // Sabit
  index++;
  packetBytes[3] = 0x52; // Sabit
  index++;

  memcpy(&packetBytes[index], &gyro_1_x, sizeof(float));
  index += sizeof(float);
  memcpy(&packetBytes[index], &gyro_1_y, sizeof(float));
  index += sizeof(float);
  memcpy(&packetBytes[index], &gyro_1_z, sizeof(float));
  index += sizeof(float);

  memcpy(&packetBytes[index], &gyro_2_x, sizeof(float));
  index += sizeof(float);
  memcpy(&packetBytes[index], &gyro_2_y, sizeof(float));
  index += sizeof(float);
  memcpy(&packetBytes[index], &gyro_2_z, sizeof(float));
  index += sizeof(float);

  packetBytes[index] = 0; //button olcak
  index++;
  packetBytes[index] = 0; //button olcak
  index++;
  packetBytes[index] = 0; //button olcak
  index++;


  packetBytes[31] = check_sum_hesapla(31); // Check_sum = check_sum_hesapla();
  packetBytes[32] = 0x0D; // Sabit
  packetBytes[32] = 0x0A; // Sabit

}