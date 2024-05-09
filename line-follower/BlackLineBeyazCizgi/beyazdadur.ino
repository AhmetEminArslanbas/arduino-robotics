
void beyazdadur() {
  
  if (sensors[0] < 200 && sensors[1] < 200 && sensors[2] < 200 && sensors[3] < 200
      && sensors[4] < 200 && sensors[5] < 200 && sensors[6] < 200 && sensors[7] < 200 ) {
         motorkontrol(50,50);
     delay(130);
     sensoroku();
     delay(5);
 if (sensors[0] > 700 && sensors[1] > 700 && sensors[2] > 700 && sensors[3] > 700
      && sensors[4] > 700 && sensors[5] > 700 && sensors[6] > 700 && sensors[7] > 700 ) {
beyazsay=beyazsay+1;
if (beyazsay==1)
{
        while (1) {
        sensoroku();
        motorkontrol(-50, 50);
        if (sensors[3] < 201 || sensors[4] < 201) {
          //ekhiz=0;
          break;
        }
      }
  }
else{
     motorkontrol(50,50);
     delay(900);
  dur();}
        }
       
  }

}
