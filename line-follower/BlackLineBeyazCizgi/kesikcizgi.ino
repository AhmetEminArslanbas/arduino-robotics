void kesikcizgi(){
  sensoroku();
  if(doksanderecesay >=11 && sensors[7] > 600 && sensors[6] > 600&& sensors[5] > 600 && sensors[4] > 600&& sensors[3] > 600 && sensors[2] > 600&& sensors[1] > 600 && sensors[0] > 600){
  motorkontrol(120,120);
  delay(1000);
}
}
