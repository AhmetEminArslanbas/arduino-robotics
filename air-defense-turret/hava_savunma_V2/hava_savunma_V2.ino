#include <AccelStepper.h>


AccelStepper stepper_ust(AccelStepper::DRIVER, 2, 3);
AccelStepper stepper_alt(AccelStepper::DRIVER, 4, 5);

int gelenDeger = 0;
int asilDeger = 0;
float motorYon = 0;
bool yon = 0;

int deger1 = 0;
int deger2 = 0;
int deger3 = 0;

float speed = 0.2;

double time = 0;

double dataTime = 0;

long ustPozisyon = 0;
long altPozisyon = 0;

int geriHiz = 2000;
bool geriMod = false;

void setup() 
{
  Serial.begin(115200);

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);

  stepper_ust.setMaxSpeed(2000);
  stepper_alt.setMaxSpeed(2000);

  stepper_ust.setCurrentPosition(0);
  stepper_alt.setCurrentPosition(0);
  //stepper.setAcceleration(600);
  time = millis();
  dataTime = millis();

  ustPozisyon = 0;
  altPozisyon = 0;
  
}

void loop() 
{
  readValue();
  
  if (millis() - dataTime < 300)
  {
    if(deger3 == 1)
    {
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
    }
    else
    {
      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);
    }
    
    stepper_ust.setSpeed(deger1);
    stepper_alt.setSpeed(deger2);

    
    /*if((deger1 > 0 && ustPozisyon > 500) || (deger1 < 0 && ustPozisyon < -500))
      stepper_ust.setSpeed(0);
    else
      stepper_ust.setSpeed(deger1);
    
    if((deger2 > 0 && altPozisyon > 1000) || (deger2 < 0 && altPozisyon < -1000))
      stepper_alt.setSpeed(0);
    else
      stepper_alt.setSpeed(deger2);*/


    geriMod = false;
  }
  else
  {
    if(!geriMod)
    {
      Serial.println("<Kamera Başlangıç Noktasına Gidiyor.>");
      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);

      geriMod = true;
    }
    
    if (ustPozisyon != 0)
      stepper_ust.setSpeed((ustPozisyon > 0) ? -geriHiz : geriHiz);
    else
      stepper_ust.setSpeed(0);

    if (altPozisyon != 0)
      stepper_alt.setSpeed((altPozisyon > 0) ? -geriHiz : geriHiz);
    else
      stepper_alt.setSpeed(0);
  }

  if (stepper_ust.runSpeed())
      ustPozisyon += (stepper_ust.speed() > 0) ? 1 : -1;

  if (stepper_alt.runSpeed())
      altPozisyon += (stepper_alt.speed() > 0) ? 1 : -1;
}

void readValue()
{
  if(Serial.available())
  {
    String veri = Serial.readStringUntil('#');
    
    int i1 = veri.indexOf('|');
    int i2 = veri.indexOf('|', i1 + 1);
    int i3 = veri.indexOf('|', i2 + 1);   // 3. | karakteri

    int v1 = veri.substring(0, i1).toInt();             // 200
    int v2 = veri.substring(i1 + 1, i2).toInt();        // 20
    int v3 = veri.substring(i2 + 1, i3).toInt();        // 110
    int v4 = veri.substring(i3 + 1).toInt();            // 45

    if((v1+v2+v3)/3 == v4)
    {
      deger1 = v1;
      deger2 = v2;
      deger3 = v3;

      Serial.read();
      Serial.flush();

      Serial.print("<");
      Serial.print(ustPozisyon);
      Serial.print("|");
      Serial.print(altPozisyon);
      Serial.println(">");

      /*Serial.print("<");
      Serial.print(deger1);
      Serial.print("|");
      Serial.print(deger2);
      Serial.print("|");
      Serial.print(deger3);
      Serial.println(">");*/

      dataTime = millis();
    }
  }
}
