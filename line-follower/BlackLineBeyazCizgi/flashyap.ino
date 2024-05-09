void flashyap(int a, int delay_1, int delay_2)
{
  for(int x=0;x<a;x++)
  {
    analogWrite(buzzer, 150);
    delay(delay_1);
    analogWrite(buzzer, 0);
    delay(delay_2);
  }
}
