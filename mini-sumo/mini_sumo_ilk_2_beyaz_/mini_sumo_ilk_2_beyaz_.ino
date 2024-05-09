#include <QTRSensors.h>


#define sag_cizgi 11 // butonlu sumo
#define sol_cizgi 12

#define on_mesafe 10
#define sag_mesafe 9
#define sol_mesafe 8

#define sag_motor_1 7
#define sag_motor_2 5
#define sag_motor_ayar 6

#define sol_motor_1 2
#define sol_motor_2 4
#define sol_motor_ayar 3

#define led_1 17
#define led_2 16
#define led_3 15

#define baslatma 18


QTRSensors qtr;

uint16_t sensorValues[2];

int ileri_speed = 255;
int arayis_hiz = 100;
int arayis_yon = 0;

int rakip_yon = 1; //1 = en son rakip sagdaydi, -1 = en son rakip soldadaydi

float zaman_1 = 0;


void setup()
{
  Serial.begin(9600);
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){11, 12}, 2);
  qtr.setEmitterPin(2);
  //delay(500);
  for (uint16_t i = 0; i < 100; i++)
  {
    qtr.calibrate();
  }
  //delay(2000);
  Serial.println("calibre etti");

  //delay(500);
  /*digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);*/
  //delay(500);


  pinMode(sag_cizgi, INPUT);
  pinMode(sol_cizgi, INPUT);
  
  
  pinMode(on_mesafe, INPUT);
  pinMode(sag_mesafe, INPUT);
  pinMode(sol_mesafe, INPUT);

  pinMode(baslatma, INPUT);

  pinMode(sag_motor_1, OUTPUT);
  pinMode(sag_motor_2, OUTPUT);
  pinMode(sag_motor_ayar, OUTPUT);


  pinMode(sol_motor_1, OUTPUT);
  pinMode(sol_motor_2, OUTPUT);
  pinMode(sol_motor_ayar, OUTPUT);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);

  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  delay(500);
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);
  delay(500);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  delay(500);
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);



  delay(2000);
  if(digitalRead(sag_mesafe) == 1)
  {
    if(digitalRead(on_mesafe) == 1)
    {
      rakip_yon = 3;
      digitalWrite(led_1, LOW);
      digitalWrite(led_3, LOW);
    }
    else if(digitalRead(sol_mesafe) == 1)
    {
      rakip_yon = 2;
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, LOW);
    }
    else
    {
      rakip_yon = 1;
      digitalWrite(led_1, LOW);
    }
  }
  else if(digitalRead(sol_mesafe) == 1)
  {
    if(digitalRead(on_mesafe) == 1)
    {
      rakip_yon = 4;
      digitalWrite(led_2, LOW);
      digitalWrite(led_3, LOW);
    }
    else
    {
      rakip_yon = -1;
      digitalWrite(led_2, LOW);
    }
    
  }
  else
  {
    rakip_yon = 1;
    digitalWrite(led_1, LOW);
  }
  

  //delay(1000); // bunun yerine kumanda komudunu yaz
  //dans
  
  //dur();
  //for(int)
  

}

void loop() 
{
  if(digitalRead(baslatma) == 1)
  {
    cizgi_kontrol();

    if(digitalRead(on_mesafe) == 1)
    {
      for(int i = 0; i <= 255; i++)
      {
        if(digitalRead(on_mesafe) == 0 || digitalRead(baslatma) == 0)
        {
          break;
        }
        ileri_speed = i;
        Serial.println(ileri_speed);

        yavastan_ileri_git();
        
        delay(5);
        while(ileri_speed == 255)
        {
          yavastan_ileri_git();

          Serial.println("ileri_speed = max speed");
          if(digitalRead(on_mesafe) == 0 || digitalRead(baslatma) == 0)
          {
            break;
          }
        }
      }


      tam_ileri_git();
      zaman_1 = millis();
      while(millis() - zaman_1 < 100)
      {
        if(digitalRead(on_mesafe) == 0 || digitalRead(baslatma) == 0)
        {
          break;
        }
      }
      while(digitalRead(on_mesafe) == 1)
      {
        ileri_git();
        if(digitalRead(baslatma) == 0)
        {
          break;
        }
      }*/
    }
    else if(digitalRead(sag_mesafe) == 1)
    {
      rakip_yon = 1;
      saga_90_don();
    }
    else if(digitalRead(sol_mesafe) == 1)
    {
      rakip_yon = -1;
      sola_90_don();
    }
    else 
    {
      if(rakip_yon == 1)
      {
        sagdan_arayis();
      }
      else if(rakip_yon == -1)
      {
        soldan_arayis();
      }
      else if(rakip_yon == 2)
      {
        saga_180_don();
        rakip_yon = 1;
      }
      else if(rakip_yon == 3)
      {
        saga_130_don();
        rakip_yon = 1;
      }
      else if(rakip_yon == 4)
      {
        sola_130_don();
        rakip_yon = -1;
      }
    }
  }
  else
  {
    dur();
  }
}

void cizgi_kontrol()
{
  uint16_t position = qtr.readLineBlack(sensorValues);

  if(sensorValues[1] <= 200 || sensorValues[0] <= 200)
  {
    if(sensorValues[1] <= 200 && sensorValues[0] <= 200)
    {
      geri_git();
      delay(200);
      saga_180_don();
    }
    else if(sensorValues[1] <= 200) // sadece sag sensor sinirdaysa 
    {
      geri_git();
      delay(200);
      sola_90_don();
      rakip_yon = -1;
    }
    else if(sensorValues[0] <= 200) // sadece sol sensor sinirdaysa 
    {
      geri_git();
      delay(200);
      saga_90_don();
      rakip_yon = 1;
    }
  }
  
  Serial.print(sensorValues[0]);
  Serial.print("  ");
  Serial.println(sensorValues[1]);
}

void yavastan_ileri_git()
{
  digitalWrite(sag_motor_1, HIGH); //yavastan ileri
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, ileri_speed);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, ileri_speed);
}
void tam_ileri_git()
{
  digitalWrite(sag_motor_1, HIGH); //tam ileri
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar,200);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, 255);
}
void ileri_git()
{
  digitalWrite(sag_motor_1, HIGH); //ileri
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, 255);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, 255);
}
void geri_git()
{
  digitalWrite(sag_motor_1, LOW); //geri
  digitalWrite(sag_motor_2, HIGH);
  analogWrite(sag_motor_ayar, 255 / 2);

  digitalWrite(sol_motor_1, LOW);
  digitalWrite(sol_motor_2, HIGH);
  analogWrite(sol_motor_ayar, 255 / 2);
}
void saga_hizli_don()
{
  digitalWrite(sag_motor_1, LOW); //saga don
  digitalWrite(sag_motor_2, HIGH);
  analogWrite(sag_motor_ayar, 255);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, 255);
}
void saga_don()
{
  digitalWrite(sag_motor_1, LOW); //saga don
  digitalWrite(sag_motor_2, HIGH);
  analogWrite(sag_motor_ayar, 255 / 2);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, 255 / 2);
}
void saga_90_don()
{
  saga_don();
  zaman_1 = millis();
  while(millis() - zaman_1 < 130)
  {
    if(digitalRead(on_mesafe) == 1)
    {
      sola_don();
      delay((millis() - zaman_1) / 4);
      break;
    }
  }
  sola_don();
  delay(30);
  dur();
  
  //saga_don();
  //delay(130);
  //sola_don();
  //delay(30);
  //dur();
}
void saga_130_don()
{
  saga_hizli_don();
  delay(130);
  sola_don();
  delay(30);
  dur();
}
void saga_180_don()
{
  saga_don();
  delay(260);
  sola_don();
  delay(30);
  dur();
}
void sola_hizli_don()
{
  digitalWrite(sag_motor_1, HIGH); //sola don
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, 255);

  digitalWrite(sol_motor_1, LOW);
  digitalWrite(sol_motor_2, HIGH);
  analogWrite(sol_motor_ayar, 255);
}
void sola_don()
{
  digitalWrite(sag_motor_1, HIGH); //sola don
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, 255 / 2);

  digitalWrite(sol_motor_1, LOW);
  digitalWrite(sol_motor_2, HIGH);
  analogWrite(sol_motor_ayar, 255 / 2);
}
void sola_90_don()
{
  sola_don();
  zaman_1 = millis();
  while(millis() - zaman_1 < 130)
  {
    if(digitalRead(on_mesafe) == 1)
    {
      saga_don();
      delay((millis() - zaman_1) / 4);
      break;
    }
  }
  saga_don();
  delay(30);
  dur();

  //delay(130);
  //saga_don();
  //delay(30);
  //dur();
}
void sola_130_don()
{
  sola_hizli_don();
  delay(130);
  saga_don();
  delay(30);
  dur();
}
void sola_180_don()
{
  sola_don();
  delay(260);
  saga_don();
  delay(30);
  dur();
}

void sagdan_arayis() //saga yavas donerek rakip ara
{
  arayis_yon = 1;
  digitalWrite(sag_motor_1, LOW); //saga yavas don
  digitalWrite(sag_motor_2, HIGH);
  analogWrite(sag_motor_ayar, arayis_hiz);

  digitalWrite(sol_motor_1, HIGH);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, arayis_hiz);

  if(digitalRead(on_mesafe) == 1)
  {
    sola_don();
    delay(20);
  }
}
void soldan_arayis() //sola yavas donerek rakip ara
{
  arayis_yon = -1;
  digitalWrite(sag_motor_1, HIGH); //sola yavas don
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, arayis_hiz);

  digitalWrite(sol_motor_1, LOW);
  digitalWrite(sol_motor_2, HIGH);
  analogWrite(sol_motor_ayar, arayis_hiz);

  if(digitalRead(on_mesafe) == 1)
  {
    saga_don();
    delay(30);
  }
}

void dur()
{
  digitalWrite(sag_motor_1, LOW); //dur
  digitalWrite(sag_motor_2, LOW);
  analogWrite(sag_motor_ayar, 0);

  digitalWrite(sol_motor_1, LOW);
  digitalWrite(sol_motor_2, LOW);
  analogWrite(sol_motor_ayar, 0);
}