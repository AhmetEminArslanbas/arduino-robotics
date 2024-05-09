#include <Wire.h>

#include "mavlink.h"
#include <SoftwareSerial.h>


// SoftwareSerial nesnelerini oluşturun
SoftwareSerial pixhawkSerial(10, 11);//rx 10, tx 11

// Setup içinde seri portu başlatın
void setup() 
{
  // MAVLink arayüzünü başlatın
  Serial.begin(57600);
  pixhawkSerial.begin(57600);
}

// Programınızı döngü içinde çalıştırın
void loop() 
{/*
  // MAVLink yapılandırması
  // MCU'nuz için varsayılan UART başlığı 
  int sysid = 1;                   ///< Bu uçağın ID'si 20. 1 PX, 255 yer istasyonu
  int compid = 158;                ///< Mesajı gönderen bileşen
  int type = MAV_TYPE_QUADROTOR;   ///< Bu sistem bir uçaktır / sabit kanatlıdır

  // Sistem tipini tanımlayın, bu durumda bir uçak -> tahta denetleyici
  uint8_t system_type = MAV_TYPE_GENERIC;
  uint8_t autopilot_type = MAV_AUTOPILOT_INVALID;
 
  uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Başlangıç ​​aşaması
  uint32_t custom_mode = 0;                 ///< Kullanıcı / benimseyici tarafından tanımlanabilir özel mod
  uint8_t system_state = MAV_STATE_STANDBY; ///< Uçuşa hazır sistem

  // Gerekli tamponları başlatın
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
 
  // Mesajı paketleyin
  mavlink_msg_heartbeat_pack(1, 0, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
 
  // Mesajı gönderi tamponuna kopyalayın
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
 
  // Standart UART gönderme işlevi ile mesajı gönderin
  unsigned long currentMillisMAVLink = millis();
  static unsigned long previousMillisMAVLink = 0;
  static unsigned long next_interval_MAVLink = 1000; // İstediğiniz zaman aralığını ayarlayabilirsiniz
  static int num_hbs_pasados = 0;
  static int num_hbs = 10; // İhtiyaca göre değiştirilebilir
  if (currentMillisMAVLink - previousMillisMAVLink >= next_interval_MAVLink) 
  {
    previousMillisMAVLink = currentMillisMAVLink;

    pixhawkSerial.write(buf, len);

    num_hbs_pasados++;
    if (num_hbs_pasados >= num_hbs) 
    {
      Mav_Request_Data();
      num_hbs_pasados = 0;
    }
  }*/
  //self.master.arducopter_arm();
delay(3000);
comm_receive();
delay(500);
comm_receive();
delay(500);
comm_receive();
delay(500);
comm_receive();

delay(15000);
sendMovementCommand(0, 0, 0, 0);
delay(5000);
sendMovementCommand(1500, 1500, 1500, 1500);
delay(5000);
sendMovementCommand(1900, 1900, 1900, 1900);
delay(5000);
sendMovementCommand(1600, 1600, 1600, 1600);
delay(5000);
sendMovementCommand(1400, 1400, 1400, 1400);
delay(5000);



  // Alınan veriyi kontrol edin
  //comm_receive();
}

void comm_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while (pixhawkSerial.available() > 0) {
    uint8_t c = pixhawkSerial.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

      switch (msg.msgid) {
        case MAVLINK_MSG_ID_RAW_IMU:
          {
            mavlink_raw_imu_t raw_imu;
            mavlink_msg_raw_imu_decode(&msg, &raw_imu);

            // Gyro verilerini yazdır
            Serial.print("Gyro X: ");
            Serial.print(raw_imu.xacc);
            Serial.print("\t Gyro Y: ");
            Serial.print(raw_imu.yacc);
            Serial.print("\t Gyro Z: ");
            Serial.println(raw_imu.zacc);
          }
          break;

        case MAVLINK_MSG_ID_RAW_PRESSURE:
          {
            mavlink_raw_pressure_t raw_pressure;
            mavlink_msg_raw_pressure_decode(&msg, &raw_pressure);

            // Pusula verilerini yazdır
            Serial.print("Pressure: ");
            Serial.println(raw_pressure.press_abs);
          }
          break;

        // Diğer case'ler buraya eklenebilir

        default:
          break;
      }
    }
  }
}


void sendMovementCommand(float roll, float pitch, float yaw, float thrust) {
    // Komut oluşturulması
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    // MAV_CMD_DO_SET_ROLL_PITCH_YAW_THRUST gibi bir komut seçilmesi
    uint16_t command = 1.9;//mavlink_msg_set_roll_pitch_yaw_thrust;

    // Komutu paketlemek için mavlink_msg_command_long_pack kullanımı
    mavlink_msg_command_long_pack(1, 200, &msg,
                                  1, 0,          // Sistem ve bileşen ID'leri
                                  command,       // Kullanılacak MAV_CMD değeri
                                  roll,          // Roll değeri (ileri-geri hareket için)
                                  pitch,         // Pitch değeri
                                  yaw,           // Yaw değeri (sağa-sola hareket için)
                                  thrust,        // Thrust değeri (ileri-geri hız kontrolü)
                                  0,             // Parametre 5
                                  0,             // Parametre 6
                                  0,             // Parametre 7
                                  0);            // Parametre 8

    // Komutu göndermek için veriyi tampona kopyala ve seri port ile gönder
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    pixhawkSerial.write(buf, len);
    Serial.println("yollandi");
}





// Veri isteği yapacak fonksiyon
void Mav_Request_Data()
{
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // İstenen AKIŞLAR
  /*
   * Tanımlamalar common.h içindedir: enum MAV_DATA_STREAM
   *   
   * MAV_DATA_STREAM_ALL=0, // Tüm veri akışlarını etkinleştir
   * MAV_DATA_STREAM_RAW_SENSORS=1, /* IMU_RAW, GPS_RAW, GPS_STATUS paketlerini etkinleştir
   * MAV_DATA_STREAM_EXTENDED_STATUS=2, /* GPS_STATUS, CONTROL_STATUS, AUX_STATUS paketlerini etkinleştir
   * MAV_DATA_STREAM_RC_CHANNELS=3, /* RC_CHANNELS_SCALED, RC_CHANNELS_RAW, SERVO_OUTPUT_RAW paketlerini etkinleştir
   * MAV_DATA_STREAM_RAW_CONTROLLER=4, /* ATTITUDE_CONTROLLER_OUTPUT, POSITION_CONTROLLER_OUTPUT, NAV_CONTROLLER_OUTPUT paketlerini etkinleştir
   * MAV_DATA_STREAM_POSITION=6, /* LOCAL_POSITION, GLOBAL_POSITION/GLOBAL_POSITION_INT mesajlarını etkinleştir
   * MAV_DATA_STREAM_EXTRA1=10, /* Uçuş kontrolörüne bağlı
   * MAV_DATA_STREAM_EXTRA2=11, /* Uçuş kontrolörüne bağlı
   * MAV_DATA_STREAM_EXTRA3=12, /* Uçuş kontrolörüne bağlı
   * MAV_DATA_STREAM_ENUM_END=13,
   * 
   * PixHawk'ta mevcut veri:
   *  - MAV_DATA_STREAM_EXTENDED_STATUS içinde Pil, amperaj ve voltaj (SYS_STATUS)
   *  - MAV_DATA_STREAM_EXTRA1 içinde Jiro bilgisi (IMU_SCALED)
   */

  // İhtiyaca göre ayarlanmalı
  const int  maxStreams = 2;
  const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_EXTENDED_STATUS, MAV_DATA_STREAM_EXTRA1};
  const uint16_t MAVRates[maxStreams] = {0x02, 0x05};
    
  for (int i = 0; i < maxStreams; i++) {
    mavlink_msg_request_data_stream_pack(2, 200, &msg, 1, 0, MAVStreams[i], MAVRates[i], 1);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    pixhawkSerial.write(buf, len);
  }
}

/*void comm_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while (pixhawkSerial.available() > 0) {
    uint8_t c = pixhawkSerial.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

      switch (msg.msgid) {
        case MAVLINK_MSG_ID_RAW_IMU:
          {
            mavlink_raw_imu_t raw_imu;
            mavlink_msg_raw_imu_decode(&msg, &raw_imu);

            // Gyro verilerini yazdır
            Serial.print("Gyro X: ");
            Serial.print(raw_imu.xgyro);
            Serial.print("\t Gyro Y: ");
            Serial.print(raw_imu.ygyro);
            Serial.print("\t Gyro Z: ");
            Serial.println(raw_imu.zgyro);
          }
          break;

        case MAVLINK_MSG_ID_RAW_PRESSURE:
          {
            mavlink_raw_pressure_t raw_pressure;
            mavlink_msg_raw_pressure_decode(&msg, &raw_pressure);

            // Pusula verilerini yazdır
            Serial.print("Pressure: ");
            Serial.println(raw_pressure.press_abs);
          }
          break;

        // Diğer case'ler buraya eklenebilir

        default:
          break;
      }
    }
  }
}*/



/*
// Veri alacak fonksiyon
void comm_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while (Serial.available() > 0) {
    uint8_t c = Serial.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

      switch (msg.msgid) {
        // Diğer case'ler buraya eklenebilir

        case MAVLINK_MSG_ID_RAW_IMU:  // #27: RAW_IMU
          {
            mavlink_raw_imu_t raw_imu;
            mavlink_msg_raw_imu_decode(&msg, &raw_imu);

            // İstenen IMU verileri burada alınabilir
            // Örneğin: raw_imu.xacc, raw_imu.yacc, raw_imu.zacc, raw_imu.xgyro, raw_imu.ygyro, raw_imu.zgyro
          }
          break;

        case MAVLINK_MSG_ID_SCALED_IMU:  // #116: SCALED_IMU2
          {
            mavlink_scaled_imu_t scaled_imu;
            mavlink_msg_scaled_imu_decode(&msg, &scaled_imu);

            // Ölçeklenmiş IMU verilerini burada alabilirsiniz
            // Örneğin: scaled_imu.xacc, scaled_imu.yacc, scaled_imu.zacc, scaled_imu.xgyro, scaled_imu.ygyro, scaled_imu.zgyro
          }
          break;

        // Diğer durumlar için gereken işlemler buraya eklenebilir

        default:
          break;
      }
    }
  }
}*/

/*// Veri alacak fonksiyon
void comm_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while (Serial.available() > 0) {
    uint8_t c = Serial.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

      switch (msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:  // #0: Heartbeat
          {
            // GCS heartbeat okuyun ve zamanlayıcı zaman aşımına girin
          }
          break;

        case MAVLINK_MSG_ID_SYS_STATUS:  // #1: SYS_STATUS
          {
            mavlink_sys_status_t sys_status;
            mavlink_msg_sys_status_decode(&msg, &sys_status);
          }
          break;

        case MAVLINK_MSG_ID_PARAM_VALUE:  // #22: PARAM_VALUE
          {
            mavlink_param_value_t param_value;
            mavlink_msg_param_value_decode(&msg, &param_value);
          }
          break;

        case MAVLINK_MSG_ID_RAW_IMU:  // #27: RAW_IMU
          {
            mavlink_raw_imu_t raw_imu;
            mavlink_msg_raw_imu_decode(&msg, &raw_imu);
          }
          break;

        case MAVLINK_MSG_ID_ATTITUDE:  // #30
          {
            int leds_modo = 0; // Global alanda leds_modo tanımlaması

            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(&msg, &attitude);

            if (attitude.roll > 1) 
              leds_modo = 0;
            else if (attitude.roll < -1) 
              leds_modo = 2;
            else 
              leds_modo = 1;
          }
          break;

        // Diğer durumlar için gereken işlemler buraya eklenebilir

        default:
          break;
      }
    }
  }
}*/
