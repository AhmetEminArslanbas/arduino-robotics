void sensor_oku()
{
  int position_1 = readline(cizgi_1_sensor, cizgi_1);

  cizgi_1[0] = map(analogRead(cizgi_1_sensor[0]), en_yuksek1_1, en_dusuk1_1, 0, 1000);
  cizgi_1[1] = map(analogRead(cizgi_1_sensor[1]), en_yuksek2_1, en_dusuk2_1, 0, 1000);
  cizgi_1[2] = map(analogRead(cizgi_1_sensor[2]), en_yuksek3_1, en_dusuk3_1, 0, 1000);
  
  hata_1 = position_1 - 1000;

  int position_2 = readline(cizgi_2_sensor, cizgi_2);

  cizgi_2[0] = map(analogRead(cizgi_2_sensor[0]), en_yuksek1_2, en_dusuk1_2, 0, 1000);
  cizgi_2[1] = map(analogRead(cizgi_2_sensor[1]), en_yuksek2_2, en_dusuk2_2, 0, 1000);
  cizgi_2[2] = map(analogRead(cizgi_2_sensor[2]), en_yuksek3_2, en_dusuk3_2, 0, 1000);
  
  hata_2 = position_2 - 1000;
  /*Serial.print("   hata: ");
  Serial.print(hata);

  Serial.print("   position: ");
  Serial.print(position);
  Serial.println("   ");*/
}

float _lastValue = 0;
int readline(unsigned int *sensor_values, unsigned int *cizgi)
{
  unsigned char i, on_line = 0;
  unsigned long avg; // this is for the weighted total, which is long
                       // before division
  unsigned int sum; // this is for the denominator which is <= 64000
  
  //readCalibrated(sensor_values, readMode);

  avg = 0;
  sum = 0;

  for(i=0; i < 3; i++) 
  {
    int value = cizgi[i];
    if(true)
      value = 1000-value; //sarı renge göre ayarala

        // keep track of whether we see the line at all
    if(value > 200) 
      on_line = 1;

        // only average in values that are above a noise threshold
    if(value > 50) 
    {
        avg += (long)(value) * (i * 1000);
        sum += value;
    }
  }

    if(!on_line)
    {
        // If it last read to the left of center, return 0.
        if(_lastValue < (3-1)*1000/2)
            return 0;

        // If it last read to the right of center, return the max.
        else
            return (3-1)*1000;

    }

    _lastValue = avg/sum;
    
    
    return _lastValue;
}