#include <QTRSensors.h>


#define sagmotor1 2
#define sagmotor2 4

#define sagmotorpwmpin 3
#define solmotor1 7
#define solmotor2 6
#define solmotorpwmpin 5



#define buzzer 8 
#define taktik 4
#define LED 13

//

double gyro_KP_yaw = 20.0; //5.0
double gyro_KI_yaw = 0.00; //0.00
double gyro_KD_yaw = 3.0; //3.0


QTRSensorsAnalog qtra((unsigned char[]) {  A7, A6, A5, A4, A3, A2, A1, A0}, 8);
unsigned int sensors[8];
float Kp = 0.055; //0.067 //0.080
float Kd = 0.53; //0.60 //0.78

int tabanhiz=100; //150 //120 // 100
int ekhiz = 0;

int sonhata = 0;
int hata = 0;
int hata_tespit = 3500;
int sagmotorpwm = 0;
int solmotorpwm = 0;
int zemin = 0; //beyaz
int taktik_durum;
int doksan_durum = 0;
int kalibrehizi = 45;
int cizgi = 0;
int taktikdurum = 0;
int doksanderecesay = 0;
int beyazsay=0;
int engel_say=0;
int yol_ayrimi_say=0;

bool doksan_donebilir = true;

float gyro_pitch = 0;
float gyro_rol = 0;
float gyro_yaw = 0;

int hedef_aci = 90;
int yaw_i_max_min_speed = 255;

struct Orientation {
  double YawAngle;
  double PitchAngle;
  double RollAngle;
};
struct IMU_Values {
  bool Error;
  bool NewDataAvailable;
  double DeltaTimeInSeconds;
  struct Orientation CurrentOrientation;
  struct Orientation PreviousOrientation;
};

void setup() {
  Serial.begin(9600);
  pinMode(sagmotor2, OUTPUT);
  pinMode(sagmotor1, OUTPUT);
  pinMode(sagmotorpwmpin, OUTPUT);
  pinMode(solmotor2, OUTPUT);
  pinMode(solmotor1, OUTPUT);
  pinMode(solmotorpwmpin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(taktik, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  //mars_setup();

  analogWrite(buzzer, 0);
  flashyap(1, 300, 300);
  for (int i = 0; i < 150; i++)
  {
    if (0 <= i && i < 5) motorkontrol(kalibrehizi, -kalibrehizi);
    if (5 <= i && i < 15) motorkontrol(-kalibrehizi+15, kalibrehizi+15);
    if (15 <= i && i < 25) motorkontrol(kalibrehizi, -kalibrehizi);
    if (25 <= i && i < 35) motorkontrol(-kalibrehizi+15, kalibrehizi+15);
    if (35 <= i && i < 40) motorkontrol(kalibrehizi, -kalibrehizi);
    if (45 <= i && i < 50) motorkontrol(-kalibrehizi+15, kalibrehizi+15);
    qtra.calibrate(); 
    delay(1);
    if (i >= 50) {
      motorkontrol(0, 0);
      delay(3);
    }
  }
  flashyap(3, 300, 300);
  delay(1000);

  /*flashyap(4, 200, 100);
  delay(500);
  initializeIMU();
  flashyap(2, 500, 100);*/

 taktik_durum = digitalRead(taktik); //12. pin taktik butonu
 if (taktik_durum == 1) {
  ekhiz = 0;
 }
 else
{
  ekhiz = 10;
 };



}
double zaman_gyro = 0;
void loop() 
{
  sensoroku();
  //doksan_durum = 0;
  //doksan();
  
  // kesikcizgi();
  //siyahtadur();
  //yol_ayrimi();

  //cizgisay();

  //beyazdadur();
  pid();  
  //engel();
  motorkontrol(solmotorpwm, sagmotorpwm);

}
