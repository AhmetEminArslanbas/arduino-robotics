#include "WiFi.h"
#include "esp_now.h"

#define motor_1_1 D9
#define motor_1_2 D10
#define motor_1_ayar A5

#define motor_2_1 D11
#define motor_2_2 D12
#define motor_2_ayar A4

float vertical_hiz = 1;
float horizontal_hiz = 1;

typedef struct struct_message 
{
  int joistic_x = 0;
  int joistic_y = 0;
} struct_message;

struct_message gelen_veriler;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&gelen_veriler, incomingData, sizeof(gelen_veriler));
  
  if(gelen_veriler.joistic_y > 5500)
  {
    //ileri git
    digitalWrite(motor_1_1, HIGH);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_y / 40 * vertical_hiz);

    digitalWrite(motor_1_1, HIGH);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_y / 40 * vertical_hiz);
  }
  else if(gelen_veriler.joistic_y < 5000)
  {
    //geri git
    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, HIGH);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_y / 40 * vertical_hiz);

    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, HIGH);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_y / 40 * vertical_hiz);
  }

  else if(gelen_veriler.joistic_x > 5500)
  {
    //saga git
    digitalWrite(motor_1_1, HIGH);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_x / 40 * horizontal_hiz);

    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, HIGH);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_x / 40 * horizontal_hiz);
  }
  else if(gelen_veriler.joistic_x < 5000)
  {
    //sola git
    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, HIGH);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_x / 40 * horizontal_hiz);

    digitalWrite(motor_1_1, HIGH);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, gelen_veriler.joistic_x / 40 * horizontal_hiz);
  }
  else
  {
    //dur
    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, 0);

    digitalWrite(motor_1_1, LOW);
    digitalWrite(motor_1_2, LOW);
    analogWrite(motor_1_ayar, 0);
  }
}

void setup() 
{
  Serial.begin(115200);

  pinMode(motor_1_1, OUTPUT);
  pinMode(motor_1_2, OUTPUT);
  pinMode(motor_1_ayar, OUTPUT);
  pinMode(motor_2_1, OUTPUT);
  pinMode(motor_2_2, OUTPUT);
  pinMode(motor_2_ayar, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("ESP-NOW başlatılamadı");
    return;
  }  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() 
{
}
