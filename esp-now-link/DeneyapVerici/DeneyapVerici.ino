#include "WiFi.h"
#include "esp_now.h"

uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0x4C, 0x58, 0x00}; // ALICI kartın MAC adresi

#define analog_joistic_x A5
#define analog_joistic_y A4

typedef struct struct_message 
{
  int joistic_x = 0;
  int joistic_y = 0;
} struct_message;

struct_message giden_veriler;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Veri gönderme başarılı" : "Veri gönderme başarısız");
}

void setup() {
  Serial.begin(115200);
  pinMode(analog_joistic_x, INPUT);
  pinMode(analog_joistic_y, INPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("ESP-NOW başlatılamadı");
    return; 
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Eşleşme başarısız");
    return;
  }
}

void loop() 
{
  
  giden_veriler.joistic_x = analogRead(analog_joistic_x);
  giden_veriler.joistic_y = analogRead(analog_joistic_y);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &giden_veriler, sizeof(giden_veriler));

  delay(10);
}
