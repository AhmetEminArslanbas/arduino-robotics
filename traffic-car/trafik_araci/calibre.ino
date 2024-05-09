void kalibrasyon_sol()
{
  if(en_yuksek1_1<analogRead(A3))
    en_yuksek1_1=analogRead(A3);
  if(en_yuksek2_1<analogRead(A4))
    en_yuksek2_1=analogRead(A4);
  if(en_yuksek3_1<analogRead(A5))
    en_yuksek3_1=analogRead(A5);

  if(en_dusuk1_1>analogRead(A3))
    en_dusuk1_1=analogRead(A3);
  if(en_dusuk2_1>analogRead(A4))
    en_dusuk2_1=analogRead(A4);
  if(en_dusuk3_1>analogRead(A5))
    en_dusuk3_1=analogRead(A5);
}

void kalibrasyon_sag()
{
  if(en_yuksek1_2<analogRead(A0))
    en_yuksek1_2=analogRead(A0);
  if(en_yuksek2_2<analogRead(A1))
    en_yuksek2_2=analogRead(A1);
  if(en_yuksek3_2<analogRead(A2))
    en_yuksek3_2=analogRead(A2);

  if(en_dusuk1_2>analogRead(A0))
    en_dusuk1_2=analogRead(A0);
  if(en_dusuk2_2>analogRead(A1))
    en_dusuk2_2=analogRead(A1);
  if(en_dusuk3_2>analogRead(A2))
    en_dusuk3_2=analogRead(A2);
}