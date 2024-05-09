void yol_ayrimi() {
    
      if (sensors[7] < 201 && sensors[0] < 201 && (sensors[3] > 701 || sensors[4] > 701 )) {
     yol_ayrimi_say=yol_ayrimi_say+1;
     if (yol_ayrimi_say==1)
     {
      while (1) {
        sensoroku();
        motorkontrol(0, 70);
        if (sensors[3] < 501 || sensors[4] < 501) {

          break;
        }
      }
      }
    }
    if (sensors[7] < 201 && sensors[0] < 201 && (sensors[3] < 201 || sensors[4] < 201 ) && (sensors[6] > 701 || sensors[5] > 701 )) {
     yol_ayrimi_say=yol_ayrimi_say+1;
     if (yol_ayrimi_say==2)
     {
      motorkontrol(0, 70);
      delay(100);
      while (1) {
        sensoroku();
        motorkontrol(0, 70);
        if (sensors[3] < 501 || sensors[4] < 501) {

          break;
        }
      }
      }
    }
}
