#include <Servo.h>

#define trigPin 4
#define echoPin 2

Servo my_servo; 

int aci = 0; 
int onceki_aci = 0;
int donus_speed = 5;
int donus_yonu = 1;
 
long zaman;
long mesafe;


float throttle = -1;
float yaw = 0;
float rol = 0;
float pitch = 0;

float btn_sd = -1;

String throttle_str;
String yaw_str;
String rol_str;
String pitch_str;

String btn_sd_str;

float sayi;
int hangi_deger_yazildi = 0;

 
void setup()
{
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin,INPUT); 
  my_servo.attach(13);
  Serial.begin(9600); 
}

void loop()
{
  servo_dondur();
  //serialEvent();
}

void serialEvent()
{
  if(Serial.available())
  {
    servo_dondur();
    /*String gelenVeri = Serial.readStringUntil('|'); // Seri porttan gelen veriyi "|" kadar oku
    gelenVeri.trim(); // Başlangıçta ve sonunda boşlukları temizle
     
    gelenVeri.replace(",", ".");

    sayi = gelenVeri.toFloat();



    if (hangi_deger_yazildi == 0) 
    {
      throttle = sayi;
      throttle_str = "throttle:" + String(sayi);
      hangi_deger_yazildi ++;
    } 
    else if(hangi_deger_yazildi == 1) 
    {
      yaw = sayi;
      yaw_str = "yaw:" + String(sayi);
      hangi_deger_yazildi ++;
    }
    else if(hangi_deger_yazildi == 2)
    {
      rol = sayi;
      rol_str = "rol:" + String(sayi);
      hangi_deger_yazildi ++;
    }
    else if(hangi_deger_yazildi == 3)
    {
      pitch = sayi;
      pitch_str = "pitch:" + String(sayi);
      hangi_deger_yazildi ++;
    }
    else if(hangi_deger_yazildi == 4)
    {
      btn_sd = sayi;
      btn_sd_str = "btn_sd:" + String(sayi);
      hangi_deger_yazildi = 0;

      /*Serial.print(throttle_str);
      Serial.print("   ");
      Serial.print(yaw_str);
      Serial.print("   ");
      Serial.print(rol_str);
      Serial.print("   ");
      Serial.print(pitch_str);

      Serial.print("   ");
      Serial.println(btn_sd_str);*/


      
      //mesafe_sensoru();
    //}
  }
}


void servo_dondur()
{
  my_servo.write(aci);

  aci += donus_yonu * donus_speed;
  if(aci >= 180)
  {
    donus_yonu = -1;
  }
  else if(aci <= 0)
  {
    donus_yonu = 1;
  }
  Serial.print(aci);
  mesafe_sensoru();
  delay(300);
  

  
}

void mesafe_sensoru()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  zaman = pulseIn(echoPin, HIGH);
  mesafe= (zaman /29.1)/2;    
  /*Serial.print("Uzaklik ");  
  Serial.print(mesafe);
  Serial.println(" cm");*/

  Serial.print("|");
  Serial.println(mesafe);

}