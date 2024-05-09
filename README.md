<div align="center">

# 🔧 Arduino Robotik Projeleri

**Yarışmalar ve kişisel projeler için yazdığım gömülü yazılımlardan bir seçki: çizgi izleyen, mini sumo, drone uçuş kontrolcüsü, İKA, MAVLink ile Pixhawk kontrolü, IoT ve daha fazlası**

![Arduino](https://img.shields.io/badge/Arduino-C++-00979D?logo=arduino&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32%20%2F%20ESP8266-E7352C?logo=espressif&logoColor=white)
![MAVLink](https://img.shields.io/badge/MAVLink-Pixhawk-blue)
![OpenCV](https://img.shields.io/badge/OpenCV-Python-5C3EE8?logo=opencv&logoColor=white)

</div>

## İçerik

| Klasör | Proje | Öne çıkanlar |
|---|---|---|
| [`line-follower`](line-follower/BlackLineBeyazCizgi) | Yarışma çizgi izleyen robotu | QTR sensör dizisi, PID ile çizgi takibi; kesikli çizgi, 90° dönüş, yol ayrımı ve siyah/beyaz zemin geçişleri; MPU6050 ile açı kontrollü dönüşler |
| [`mini-sumo`](mini-sumo) | Mini sumo robotu | Sağdan/soldan rakip arama, 90°/130°/180° hızlı dönüş manevraları, ring çizgisini algılayıp içeride kalma; beyaz ve siyah ring için iki sürüm |
| [`traffic-car`](traffic-car/trafik_araci) | Trafik parkuru için otonom araç | Çizgi sensörü kalibrasyonu, PID ile şerit takibi, motor shield ile sürüş, sesli/ışıklı uyarı |
| [`drone-flight-controller`](drone-flight-controller) | Arduino tabanlı quadcopter uçuş kontrolcüsü | MPU6050 DMP ile yönelim, roll/pitch/yaw için PID, motor karışımı (mixer), FUTABA SBUS kumanda girişi, watchdog |
| [`pixhawk-mavlink`](pixhawk-mavlink/pixhawk_control_arduino) | Arduino → Pixhawk MAVLink kontrolü | MAVLink heartbeat, veri akışı isteği, `COMMAND_LONG` ile `DO_SET_ROLL_PITCH_YAW_THRUST` komutları |
| [`ugv`](ugv) | İnsansız kara aracı (İKA) sürüş yazılımı | SBUS/RC alıcıdan komut okuma, ön teker direksiyon kontrolü, IMU destekli dönüş sistemi, kumanda kaybında güvenli durma |
| [`air-defense-turret`](air-defense-turret) | Görüntü işlemeli hava savunma tareti | Python + OpenCV ile HSV renk hedef takibi, PID ile pan/tilt hata düzeltme, seri hat üzerinden AccelStepper ile iki eksenli step motor kontrolü |
| [`iot-color-analyzer`](iot-color-analyzer/E-Tahlil) | E-Tahlil: IoT renk analiz cihazı | TCS34725 renk sensörüyle ölçüm, LCD arayüzü, ESP8266 ile sonuçların ThingSpeak bulutuna gönderilmesi |
| [`esp-now-link`](esp-now-link) | ESP-NOW kablosuz kumanda bağlantısı | Wi-Fi ağı olmadan iki ESP32 (Deneyap Kart) arasında düşük gecikmeli veri iletimi |
| [`ultrasonic-radar`](ultrasonic-radar/radar) | Ultrasonik radar | Servo ile tarama yapan mesafe sensörü, açı-mesafe verisinin seri porttan aktarılması |
| [`gyro-robot-arm`](gyro-robot-arm/robot_kol_gyro) | Jiroskopla kontrol edilen robot kol | El hareketi (IMU) ile servo eklemlerinin eşlenmesi |

## Kullanılan donanım ve konular
- **Mikrodenetleyiciler:** Arduino Uno, Nano, ESP32 (Deneyap), ESP8266
- **Sensörler:** MPU6050, QTR yansıma sensörleri, TCS34725 renk sensörü, HC-SR04, SBUS alıcılar
- **Eyleyiciler:** ESC/BLDC, DC motor sürücüleri, servo, step motor (AccelStepper)
- **Haberleşme:** UART, I2C, SBUS, MAVLink, ESP-NOW, HTTP (ThingSpeak)
- **Kontrol:** PID, motor karışımı, durum makineleri

## Çalıştırma
Her alt klasör, Arduino IDE ile doğrudan açılabilen bir taslaktır (klasör adı = ana `.ino` adı). Gerekli kütüphaneler dosyaların başındaki `#include` satırlarında görülebilir. Ağ bilgileri ve API anahtarları `YOUR_...` yer tutucularıyla değiştirilmiştir.

> Daha kapsamlı ve belgelenmiş projelerim için profilimdeki sabitlenmiş repolara bakabilirsiniz: denge robotu, ROV, roket yer istasyonu ve FFB direksiyon.

---
<sub>© Ahmet Emin Arslanbaş — Tüm hakları saklıdır.</sub>
