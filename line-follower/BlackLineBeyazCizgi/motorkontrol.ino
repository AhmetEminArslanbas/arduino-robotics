void motorkontrol(int solmotorpwm,int sagmotorpwm){   //Örnek: motorkontrol(-50,50);

if(solmotorpwm<=0){
  solmotorpwm=abs(solmotorpwm);
  digitalWrite(solmotor1,LOW);
  digitalWrite(solmotor2,HIGH);
  analogWrite(solmotorpwmpin,solmotorpwm);
}
else{
  digitalWrite(solmotor1,HIGH);
  digitalWrite(solmotor2,LOW);
  analogWrite(solmotorpwmpin,solmotorpwm);
}

 if(sagmotorpwm<=0){
  sagmotorpwm=abs(sagmotorpwm);
  digitalWrite(sagmotor1,LOW);
  digitalWrite(sagmotor2,HIGH);
  analogWrite(sagmotorpwmpin,sagmotorpwm);
 }
 else{
  digitalWrite(sagmotor1,HIGH);
  digitalWrite(sagmotor2,LOW);
  analogWrite(sagmotorpwmpin,sagmotorpwm);
 }
}
