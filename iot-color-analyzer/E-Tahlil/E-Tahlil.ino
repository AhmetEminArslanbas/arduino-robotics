  #include <LiquidCrystal.h>
  const int rs = D3, en = D4, d4 = D0, d5 = D7, d6 = D6, d7 = D5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "Adafruit_TCS34725.h" //RENK SENSÖRÜ İÇİN KÜTÜPHANE

#include <ThingSpeak.h>
int frequency = 0;
int red,green,blue;
float re,ge,be;
uint16_t r, g, b, c, colorTemp, lux;//RENK SENSÖRÜ İÇİN GEREKLİ OLAN DEĞİŞKENLER
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_1X);
const char* ssid = "YOUR_WIFI_SSID";   // Your Network SSID
const char* password = "YOUR_WIFI_PASSWORD";       // Your Network Password

WiFiClient client;
unsigned long myChannelNumber = 0; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "YOUR_THINGSPEAK_WRITE_KEY"; //Your Write API Key
 int sayim = 1;

String key = "ığüşöç";
byte o[7][8] = {
  {
  B00000,
  B00000,
  B00000,
  B00110,
  B00010,
  B00010,
  B00010,
  B00010
  },
  {
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110},
  {
  B00000,
  B01010,
  B00000,
  B10001,
  B10001,
  B10001,
  B10011,
  B01101
},
{
  B00000,
  B01110,
  B10000,
  B01110,
  B00001,
  B00101,
  B11110,
  B00100
},
{
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110
},
{
  B00100,
  B01010,
  B10001,
  B00000,
  B00100,
  B01010,
  B10001,
  B00000
},
  {
  B00000,
  B00100,
  B00010,
  B11111,
  B11111,
  B00010,
  B00100,
  B00000
  }
} ;



void yaz(String data){
   char arr[data.length()+1];
    data.toCharArray(arr, data.length()+1);
 
    for(int i = 0; i < data.length(); i++){
      char a = arr[i];
      
       if(a == 69)
       lcd.write(byte(0));
      
      }
  
  }

 
void setup() {  
  pinMode(D8, INPUT);
  pinMode(3, INPUT);
 // pinMode(10, INPUT);
  Serial.begin(115200);  
  lcd.setCursor(2, 0);
  lcd.clear();
  lcd.setCursor(1, 0);
    lcd.begin(16, 2);
lcd.createChar(0, o[0]);
lcd.createChar(1, o[1]);
lcd.createChar(2, o[2]);
lcd.createChar(3, o[3]);
lcd.createChar(4, o[4]);
lcd.createChar(5, o[5]);
lcd.createChar(6, o[6]);
 
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);


for(int i = 0; i < 20; i++){

    Serial.print(".");  
    lcd.clear();
    lcd.println("baglaniyor"+getPoint());
   delay(1000);  
   lcd.setCursor(0, 1);
   lcd.print(20-i);
   if (WiFi.status() == WL_CONNECTED)
{
  lcd.clear();
  lcd.setCursor(15, 0);
  lcd.write(byte(5));
  lcd.print("Baglandi!");
  break;
}
  }  
  lcd.clear();
  //lcd.print("Baglandi!");
  yaz("hello");
  delay(300);
  Serial.println();  
  Serial.print("connected: ");  
  Serial.println(WiFi.localIP());  
 
  delay(1000);
}  
String getPoint(){
  if(sayim<3){
      sayim++;
      }else{
        sayim=1;
        }
        return givePoint(sayim);
  }
String givePoint(int p){
  String point = "";

  for(int i = 0; i < p; i++){
    point=point+".";
    }
    return point+"     ";
  }
  
int n = 0;  
int mod=0;
void loop() { 
  if (WiFi.status() == WL_CONNECTED)
{
  lcd.setCursor(15, 0);
  lcd.write(byte(5));
} 
  if (mod==0)
  okuma();
  if (mod==1) 
  menu();
  if (mod==2)
  tansiyon();
  if (mod==3) 
  acil_durum();
  if (mod==4)
  buyuk_tansiyon();
  if (mod==5) 
  dusuk_tansiyon();
}


 void okuma(){ 
  delay(200);
   if (analogRead(A0)>50)
   mod=1;
   tcs.getRawData(&r, &g, &b, &c);
   int arti;
  
    Serial.print("re=");
   Serial.print(re);
    Serial.print("   ge=");
   Serial.print(ge);
    Serial.print("   be=");
   Serial.print(be);
  Serial.print(" c=");
   Serial.println(c);
     //  lcd.setCursor(0, 1);
      //  lcd.print(c);
    
       
   
   if (c>15000){
  lcd.clear();
  lcd.setCursor(0, 0);
   int sayii = 1;
   lcd.clear();
   lcd.print("hesaplaniyor"+givePoint(1));
   delay(3000);
   for(int i = 1; i <=3; i++){
   lcd.clear();
   lcd.print("hesaplaniyor"+givePoint(i));
   delay(1000);
   tcs.getRawData(&r, &g, &b, &c);
   colorTemp = tcs.calculateColorTemperature(r, g, b);
   lux = tcs.calculateLux(r, g, b);
   float ortalama = (r+g+b)/3; // renk değerlerini topla ve ortalamasını al
   re = r/ortalama+re; // kırmızı renk değeri oranı
   ge = g/ortalama+ge; // yeşil renk değeri oranı
   be = b/ortalama+be; // mavi renk değeri oranı
   lcd.clear();
   lcd.setCursor(0, 0);
   }
   re=re/3;
   be=be/3;
   ge=ge/3;
    Serial.print("re=");
    Serial.print(re);
    Serial.print("   ge=");
    Serial.print(ge);
    Serial.print("   be=");
    Serial.print(be);
    Serial.print(" c=");
    Serial.println(c);
   
   if (ge>1.27 and ge<1.32 and re>0.78 and re<0.98 and be>0.73 and be<0.92)
   {
 
     arti=3;
     lcd.print("sonuc +++");
     lcd.setCursor(0, 1);
     lcd.print("R");
     lcd.setCursor(1, 1);
     lcd.print(re);
     lcd.setCursor(6, 1);
     lcd.print("G");
     lcd.setCursor(7, 1);
     lcd.print(ge);
     lcd.setCursor(11, 1);
     lcd.print("B");
     lcd.setCursor(12, 1);
     lcd.print(be);
     arti=3;
   }
   else  if (ge>1.29 and ge<1.31 and re>0.84 and re<0.97 and be>0.72 and be<0.84)
   {

  
     arti=2;
     lcd.print("sonuc ++");
     lcd.setCursor(0, 1);
     lcd.print("R");
     lcd.setCursor(1, 1);
     lcd.print(re);
     lcd.setCursor(6, 1);
     lcd.print("G");
     lcd.setCursor(7, 1);
     lcd.print(ge);
     lcd.setCursor(11, 1);
     lcd.print("B");
     lcd.setCursor(12, 1);
     lcd.print(be);
   }
  else if (ge>1.24 and ge<1.30 and re>0.94 and re<1.05 and be>0.67 and be<0.80)
   {
     arti=1;
     lcd.print("sonuc +");
     lcd.setCursor(0, 1);
     lcd.print("R");
     lcd.setCursor(1, 1);
     lcd.print(re);
     lcd.setCursor(6, 1);
     lcd.print("G");
     lcd.setCursor(7, 1);
     lcd.print(ge);
     lcd.setCursor(11, 1);
     lcd.print("B");
     lcd.setCursor(12, 1);
     lcd.print(be);
     arti=1;
   }
 else  if (ge>1.18 and ge<1.30 and re>1 and re<1.15 and be>0.65 and be<0.80)
   {
     arti=0;
     lcd.print("sonuc -");
     lcd.setCursor(0, 1);
     lcd.print("R");
     lcd.setCursor(1, 1);
     lcd.print(re);
     lcd.setCursor(6, 1);
     lcd.print("G");
     lcd.setCursor(7, 1);
     lcd.print(ge);
     lcd.setCursor(11, 1);
     lcd.print("B");
     lcd.setCursor(12, 1);
     lcd.print(be);
     arti=0;
   }
   else{
     arti=10;
    lcd.print("hesaplanamadi");
     lcd.setCursor(0, 1);
     lcd.print("R");
     lcd.setCursor(1, 1);
     lcd.print(re);
     lcd.setCursor(6, 1);
     lcd.print("G");
     lcd.setCursor(7, 1);
     lcd.print(ge);
     lcd.setCursor(11, 1);
     lcd.print("B");
     lcd.setCursor(12, 1);
     lcd.print(be);
    }
    re=0;
    be=0;
    ge=0;
    delay(3000);
   if (arti<5)
     ThingSpeak.writeField(myChannelNumber, 1,arti, myWriteAPIKey); //Update in ThingSpeak
  
      delay(7000);
      lcd.clear();
      if (WiFi.status() == WL_CONNECTED)
{
      lcd.setCursor(0, 0);
      lcd.print("veri gonderildi");
} 
else 
{
      lcd.setCursor(0, 0);
      lcd.print("veri gitmedi");
      lcd.setCursor(0, 1);
      lcd.print("wifi bagli degil");
} 
     
         delay(5000);
          lcd.clear();
          int bir = 1;
          while(c>15000){
            delay(1000);
            lcd.clear();
                
             lcd.print("stribi cikarin");
             lcd.setCursor(0, 0);
             
             lcd.setCursor(0, 1);
             lcd.print(++bir);
             tcs.getRawData(&r, &g, &b, &c);
          } 
             lcd.clear();
             lcd.setCursor(0, 0);
             lcd.print("tamamlandi");
             lcd.setCursor(0, 1);
              lcd.print("iyi gunler");
             delay(5000);
        
      // Free resources
      
    }

  
  
 /*  
   lcd.setCursor(0, 0);
    lcd.print("R");
    lcd.setCursor(1, 0);
    lcd.print(re);
       lcd.setCursor(6, 0);
    lcd.print("G");
     lcd.setCursor(7, 0);
    lcd.print(ge);
       lcd.setCursor(12, 0);
    lcd.print("B");
     lcd.setCursor(13, 0);
    lcd.print(be);*/
   else{
    delay(100);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("stribi");
    lcd.setCursor(0, 1);
    lcd.print("yerlestiriniz");
     //lcd.print(digitalRead(D8));
    // lcd.print(analogRead(A0));
    
    }
   
  
  
}
int mod1=0;
unsigned long zaman;
unsigned long zaman1;

void menu()
{
  delay(200);

  
  if (mod1==0)
  {
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("ust menu");
  lcd.setCursor(1, 1);
  lcd.print("tansiyon");
  }
 if(mod1==1)
    {
        lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6)); 
  lcd.setCursor(1, 0);
  lcd.print("tansiyon");
  lcd.setCursor(1, 1);
  lcd.print("acil durum");
  }
  if(mod1==2)
    {
        lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("tansiyon");
  lcd.setCursor(1, 1);
  lcd.print("acil durum");
  }
  if (analogRead(A0)>50)
  {
    delay(200);
    if(mod1<2)
    mod1++;
    else
    mod1=0;
      }
  if (digitalRead(D8)==1)
  {
if (mod1==0)
  mod=0;
if (mod1==1)
  mod=2;
if (mod1==2)
  mod=3;

 mod1=0;
  }
 }  

void tansiyon()
{
  delay(200);

  
  if (mod1==0)
  {
     lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("ust menu");
  lcd.setCursor(1, 1);
  lcd.print("yuksek");
  }
 if(mod1==1)
    {
        lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6)); 
  lcd.setCursor(1, 0);
  lcd.print("yuksek");
  lcd.setCursor(1, 1);
  lcd.print("alcak");
  }
  if(mod1==2)
    {
        lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("yuksek");
  lcd.setCursor(1, 1);
  lcd.print("alcak");
  }
  if (analogRead(A0)>50)
  {
    delay(200);
    if(mod1<2)
    mod1++;
    else
    mod1=0;
      }
  if (digitalRead(D8)==1)
  {
if (mod1==0)
  mod=1;
if (mod1==1)
  mod=4;
if (mod1==2)
  mod=5;

 mod1=0;
  }
}
void acil_durum()
{
delay(200);

  
  if (mod1==0)
  {
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("ust menu");
  lcd.setCursor(1, 1);
  lcd.print("bas agrisi");
  }
 if(mod1==1)
    {
        lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6)); 
  lcd.setCursor(1, 0);
  lcd.print("bas agrisi");
  lcd.setCursor(1, 1);
  lcd.print("yuksek tansiyon");
  }
  if(mod1==2)
    {
        lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("bas agrisi");
  lcd.setCursor(1, 1);
  lcd.print("yuksek tansiyon");
  }
  if (analogRead(A0)>50)
  {
    delay(200);
    if(mod1<2)
    mod1++;
    else
    mod1=0;
      }
  if (digitalRead(D8)==1)
  {
if (mod1==0)
  mod=1;
if (mod1==1)
    {
        lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("bas agrisi");
  lcd.setCursor(1, 1);
   ThingSpeak.writeField(myChannelNumber, 4,1, myWriteAPIKey); //Update in ThingSpeak
  

      lcd.clear();
 
            if (WiFi.status() == WL_CONNECTED)
{
      lcd.setCursor(0, 0);
      lcd.print("veri gonderildi");
} 
else 
{
      lcd.setCursor(0, 0);
      lcd.print("veri gitmedi");
      lcd.setCursor(0, 1);
      lcd.print("wifi bagli degil");
} 
      
          

  delay(2000);
 mod1=0;
  }
if (mod1==2)
    {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("yuksek tansiyon");
  lcd.setCursor(1, 1);
     ThingSpeak.writeField(myChannelNumber, 4,2, myWriteAPIKey); //Update in ThingSpeak
  

      lcd.clear();
  
       if (WiFi.status() == WL_CONNECTED)
{
      lcd.setCursor(0, 0);
      lcd.print("veri gonderildi");
} 
else 
{
      lcd.setCursor(0, 0);
      lcd.print("veri gitmedi");
      lcd.setCursor(0, 1);
      lcd.print("wifi bagli degil");
} 
  delay(2000);
 mod1=0;
  }

  }
}
void buyuk_tansiyon()
{
delay(200);

if (mod1==0)
  {
     lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("ust menu");
  lcd.setCursor(1, 1);
  lcd.print("B.T.=  10.00");
  }
  if (mod1==17)
  {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("B.T.= 18.00");
  lcd.setCursor(1, 1);
  lcd.print("B.T.= >18.00");
  }
 for (float i=1; i<17; i++)
  {
   if(mod1==i)
   {
    
  float t=0;
    t=(i/2)+9.5;
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("B.T.= ");
  lcd.setCursor(8, 0);
  lcd.print(t);
  lcd.setCursor(1, 1);
  lcd.print("B.T.= ");
  lcd.setCursor(8, 1);
  lcd.print(t+0.5);
   }
     
  }

  if (analogRead(A0)>50)
  {
    delay(200);
    if(mod1<17)
    mod1++;
    else
    mod1=0;
      }
  if (digitalRead(D8)==1)
  {
  if (mod1==0)
  mod=2;
  for (float i=1; i<17; i++)
  {
   if(mod1==i)
   {
  float t=0;
    t=(i/2)+9.5;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("B.Tansiyon= ");
  lcd.setCursor(14, 0);
  lcd.print(t);
     ThingSpeak.writeField(myChannelNumber, 2,t, myWriteAPIKey); //Update in ThingSpeak
  

      lcd.clear();
              if (WiFi.status() == WL_CONNECTED)
{
      lcd.setCursor(0, 0);
      lcd.print("veri gonderildi");
} 
else 
{
      lcd.setCursor(0, 0);
      lcd.print("veri gitmedi");
      lcd.setCursor(0, 1);
      lcd.print("wifi bagli degil");
} 

  mod1=0;
  mod=0;
  delay(3000);
   }
     
  }
  }
}
void dusuk_tansiyon()
{
  delay(200);

if (mod1==0)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("ust menu");
  lcd.setCursor(1, 1);
  lcd.print("B.T.=  2.00");
  }
  if (mod1==17)
  {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("B.T.=  10.00");
  lcd.setCursor(1, 1);
  lcd.print("B.T.= >10.00");
  }
 for (float i=1; i<17; i++)
  {
   if(mod1==i)
   {
    
  float t=0;
    t=(i/2)+1.5;
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.setCursor(1, 0);
  lcd.print("B.T.= ");
  lcd.setCursor(8, 0);
  lcd.print(t);
  lcd.setCursor(1, 1);
  lcd.print("B.T.= ");
  lcd.setCursor(8, 1);
  lcd.print(t+0.5);
   }
     
  }

  if (analogRead(A0)>50)
  {
    delay(200);
    if(mod1<17)
    mod1++;
    else
    mod1=0;
      }
  if (digitalRead(D8)==1)
  {
  if (mod1==0)
  mod=2;
  for (float i=1; i<17; i++)
  {
   if(mod1==i)
   {
  float t=0;
    t=(i/2)+2.5;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("K.Tansiyon= ");
  lcd.setCursor(14, 0);
  lcd.print(t);
     ThingSpeak.writeField(myChannelNumber, 3,t, myWriteAPIKey); //Update in ThingSpeak
  

      lcd.clear();
        if (WiFi.status() == WL_CONNECTED)
{
      lcd.setCursor(0, 0);
      lcd.print("veri gonderildi");
} 
else 
{
      lcd.setCursor(0, 0);
      lcd.print("veri gitmedi");
      lcd.setCursor(0, 1);
      lcd.print("wifi bagli degil");
} 

  mod1=0;
  mod=0;
  delay(3000);
   }
     
  }
  }
}