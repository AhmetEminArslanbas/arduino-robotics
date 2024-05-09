void cizgisay() {
    
    if(sensors[7] > 700 && sensors[6] > 700 && sensors[5] > 700 && sensors[4] > 700 && sensors[3] > 700 && sensors[2] > 700&& sensors[1] > 700 && sensors[0] > 700)
    {
      cizgi++;
      motorkontrol(70, 70);
      delay(30);
      if(cizgi == 1)
      {
        //Kp = 0.06;
        //Kd = 0.6;

        //tabanhiz=150;
      }
    }
      
}
