#include <Servo.h>
#include <AFMotor.h>

AF_DCMotor sag_motor(4);
AF_DCMotor sol_motor(2);
AF_DCMotor buzzer(3);

Servo kamera_servo;

#define on_mesafe 9

int cizgi_1_sensor[] = {A3, A4, A5};
int cizgi_1[] = {0, 0, 0};

int cizgi_2_sensor[] = {A0, A1, A2};
int cizgi_2[] = {0, 0, 0};

int hata_1 = 1000;
int hata_2 = 1000;

int total_hata = 0;

double KP_yaw = 0.2; //0.1
double KI_yaw = 0.00; //0
double KD_yaw = 5.5; //2

float max_toparlama_speed = 255;

float taban_hiz = 150;
int kalibrehizi = 75;

bool arac_durdur = false;

int en_yuksek1_1=0;
int en_yuksek2_1=0;
int en_yuksek3_1=0;
int en_dusuk1_1=1000;
int en_dusuk2_1=1000;
int en_dusuk3_1=1000;

int en_yuksek1_2=0;
int en_yuksek2_2=0;
int en_yuksek3_2=0;
int en_dusuk1_2=1000;
int en_dusuk2_2=1000;
int en_dusuk3_2=1000;

int yon_durum = 1;
double yon_zaman = 0;
double zaman = 0;

void setup() 
{
  Serial.begin(9600);
  
  for (int i=0; i<3; i++)
  {
    pinMode(cizgi_1_sensor[i], INPUT);
    pinMode(cizgi_2_sensor[i], INPUT);
  }

  pinMode(on_mesafe, INPUT);
  kamera_servo.attach(10);
  kamera_servo.write(20);

  sag_motor.run(RELEASE);
  sol_motor.run(RELEASE);

  buzzer.run(FORWARD);
  buzzer.setSpeed(0);

  //Serial.println("calibre basladi");
  buzzer_flash(5, 300);
  for (int i=0; i<400; i++)
  {
    if (0 <= i && i < 35) motor_hareket(kalibrehizi, -kalibrehizi);
    if (35 <= i && i < 85) motor_hareket(-kalibrehizi, kalibrehizi);
    if (85 <= i && i < 125) motor_hareket(kalibrehizi, -kalibrehizi);
    if (125 <= i && i < 145) motor_hareket(-kalibrehizi, kalibrehizi);
    if (145 <= i && i < 165) motor_hareket(kalibrehizi, -kalibrehizi);
    if (165 <= i && i < 185) motor_hareket(-kalibrehizi, kalibrehizi);

    if (185 <= i && i < 200) motor_hareket(0, 0);
    kalibrasyon_sag(); // içine soldaki cizgi sensörlerini de ekle
    delay(10);
  }
  motor_hareket(-80 ,-160);
  delay(150);
  motor_hareket(0, 0);
  delay(3000);

  buzzer_flash(5, 300);
  for (int i=0; i<400; i++)
  {
    if (0 <= i && i < 35) motor_hareket(kalibrehizi, -kalibrehizi);
    if (35 <= i && i < 85) motor_hareket(-kalibrehizi, kalibrehizi);
    if (85 <= i && i < 125) motor_hareket(kalibrehizi, -kalibrehizi);
    if (125 <= i && i < 145) motor_hareket(-kalibrehizi, kalibrehizi);
    if (145 <= i && i < 165) motor_hareket(kalibrehizi, -kalibrehizi);
    if (165 <= i && i < 185) motor_hareket(-kalibrehizi, kalibrehizi);

    if (185 <= i && i < 200) motor_hareket(0, 0);
    kalibrasyon_sol(); // içine soldaki cizgi sensörlerini de ekle
    delay(10);
  }

  buzzer_flash(3, 200);
  //Serial.println("calibre bitti");
  

  /*while(1)
  {
    if(Serial.available())
    {
      char veri = Serial.read();
      //Serial.println(veri);
      if(veri == '3')
        break;
    }
  }*/
  kamera_servo.write(110);
  buzzer_flash(2, 500);
  kamera_servo.write(110);
  delay(500);
  zaman = millis();
}

void loop() 
{
  if(digitalRead(on_mesafe) == 0 && yon_durum == 1)
  {
    kamera_servo.write(20);
    motor_hareket(0, 0);
          delay(500);
            motor_hareket(-150, 150);
            delay(200);
            motor_hareket(150, -150);
            delay(5);
            motor_hareket(80, 160);
            while(1)
            {
              sensor_oku();
              if(cizgi_1[0] < 200 || cizgi_1[1] < 200 || cizgi_1[2] < 200)
              {
                yon_durum = -1;
                yon_zaman = millis();
                break;
              }
            }
  }
  /*if(millis() - zaman > 100)
  {
    Serial.println(total_hata);
    zaman = millis();
  }*/
  /*if(digitalRead(on_mesafe) == 0)
  {
    //arac_durdur = true;
    motor_hareket(0, 0);
    Serial.println(5);
    while(1)
    {
      if(Serial.available())
      {
        char veri = Serial.read();
        if(veri == '2') //3
        {
          buzzer_flash(3, 200);
        }

        else if(veri == '5') //saga don
        {
          buzzer_flash(5, 200);
          kamera_servo.write(20);
          motor_hareket(0, 0);
          delay(500);
            motor_hareket(-150, 150);
            delay(275);
            motor_hareket(150, -150);
            delay(10);
            motor_hareket(80, 160);
            while(1)
            {
              sensor_oku();
              if(cizgi_2[0] < 200 || cizgi_2[1] < 200 || cizgi_2[2] < 200)
              {
                kamera_servo.write(110);
                break;
              }
            }
            break;
        }
        /*switch(veri)
        {
          case '6':
          {
            kamera_servo.write(110);//kamera kapat
            delay(500);
            motor_hareket(-150, 150);
            delay(750);
            motor_hareket(150, -150);
            delay(20);
            motor_hareket(120, 120);
            while(1)
            {
              sensor_oku();
              if(cizgi_2[0] < 200 || cizgi_2[1] < 200 || cizgi_2[2] < 200)
              {
                break;
              }
            }
            break;
          }

        }*/
      /*}
    }
  }
  else
  {
    arac_durdur = false;
    kamera_servo.write(110);
  }*/
  sensor_oku();

  if(cizgi_2[2] > 1200)
  {
    motor_hareket(-130, 100);
    delay(100);
    while(1)
    {
      sensor_oku();
      if(cizgi_2[0] < 200)
        break;
    }
  }
 

  if(yon_durum == 1)
  {
    float pid_deger = yaw_pid_hesapla(hata_2);
    //Serial.println(pid_deger);
    kamera_servo.write(110);
    if(arac_durdur)
    {
      motor_hareket(pid_deger*1.6, -pid_deger*1.6);
    }
    else
    {
      motor_hareket(taban_hiz + pid_deger, taban_hiz - pid_deger);
    }
  }
  else if(yon_durum = -1)
  {
    float pid_deger = yaw_pid_hesapla(hata_1);
    motor_hareket(taban_hiz + pid_deger, taban_hiz - pid_deger);
    if(millis() - yon_zaman > 4000)
    {
      motor_hareket(150, -150);
            delay(200);
            motor_hareket(-150, 150);
            delay(5);
            motor_hareket(80, 160);
      while(1)
            {
              sensor_oku();
              if(cizgi_2[0] < 200 || cizgi_2[1] < 200 || cizgi_2[2] < 200)
              {
                yon_durum = 1;
                zaman = millis();
                break;
              }
            }
    }
  }
}
