double doksan_zaman = 0;
bool altigende = false;
void doksan() 
{
    //beyaz zemin sağa doksan derece
    if ((millis() - doksan_zaman) > 1000 && sensors[7] > 700 && sensors[6] > 700 && sensors[5] > 700 && /*sensors[4] > 700 &&*/ sensors[1] < 300 && sensors[0] < 300) 
    {
      analogWrite(buzzer, 150);

      motorkontrol(-70, -70);
      delay(100); //30
     // while (1) {
     //   sensoroku();
      //  if (sensors[3] > 500 && sensors[4] > 500) {
     //     break;
      //  }
     // }
      
      //motorkontrol(0, 50); //göbekten dönmesi için
      //delay(200);
      while (1) {
        sensoroku();
        motorkontrol(-50, 50);
        if (sensors[3] > 700 || sensors[4] > 700) 
        {
          //motorkontrol(70, -70);
          //delay(10);
          //ekhiz=0;
          doksan_durum = 1;
          doksanderecesay++;
          doksan_zaman = millis();
          analogWrite(buzzer, 0);
          break;
        }
      }
    }

  if ((millis() - doksan_zaman) > 1000 && sensors[0] > 700 && sensors[1] > 700 && sensors[2] > 700 && /*sensors[3] > 700 &&*/  sensors[6] < 300 && sensors[7] < 300) {

    analogWrite(buzzer, 150);
    motorkontrol(-70, -70);
    delay(100);

      //while (1) {
      //  sensoroku();
       // if (sensors[3] > 500 && sensors[4] > 500) {
       //   break;
      //  }
     // }
      while (1) {
        sensoroku();
        motorkontrol(50, -50);
        if (sensors[3] > 700 || sensors[4] > 700) 
        {
          //motorkontrol(-70, 70);
          //delay(10);
          //ekhiz=0;
          doksan_durum = 1;
          doksanderecesay++;
          doksan_zaman = millis();
          analogWrite(buzzer, 0);
          break;
        }
      }
    }

  if(doksanderecesay == 5)
  {
    //dur();
  }
}
