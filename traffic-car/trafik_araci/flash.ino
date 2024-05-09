void buzzer_flash(int tekrar, int gecikme)
{
  for(int i=0; i<tekrar; i++)
  {
    buzzer.run(FORWARD);
    buzzer.setSpeed(150);
    delay(gecikme);
    buzzer.run(FORWARD);
    buzzer.setSpeed(0);
    delay(gecikme);
  }
  buzzer.setSpeed(0);
}