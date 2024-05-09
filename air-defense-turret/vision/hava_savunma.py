import serial
import time
import cv2
import numpy as np
import threading
from pid import PID
import math
import keyboard


#cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
cap = cv2.VideoCapture(0)

ser = serial.Serial("COM11", 115200, timeout=1)


def arduino_veri_gonder(veri):
    try:
        ser.write(veri.encode('utf-8'))
        #print("Gönderilen veri:", veri)
    except Exception as e:
        print("Hata:", e)
def arduino_veri_oku():
    try:
        if ser.in_waiting > 0:
            gelen = ser.readline().decode('utf-8').strip()
            if gelen != "":
                print("Arduino:", gelen)
    except Exception as e:
        print("Okuma hatası:", e)


pidX = PID(kp=15, ki=0, kd=14) #(kp=15, ki=1, kd=10)
pidY = PID(kp=8, ki=0, kd=12) #(kp=8, ki=0.5, kd=8)


servo1 = 0
servo2 = 0

pixelX = 0
pixelY = 0

x_prev = 0
y_prev = 0
t_prev = time.time()
t_now = time.time()

fireTime = time.time()
isFire = False

camWith = 0
camHeight = 0
role = 0

olcek = 2

lower_red1 = np.array([0, 120, 70])
upper_red1 = np.array([10, 255, 255])

lower_red2 = np.array([170, 120, 70])
upper_red2 = np.array([179, 255, 255])

lower_blue = np.array([90, 50, 40])
upper_blue = np.array([140, 255, 255])

lower_green = np.array([35, 50, 50])
upper_green = np.array([85, 255, 255])

isManual = False

isRun = True

def arduino_thread():
    while isRun:
        arduino_veri_gonder(str(servo1) + "|" + str(servo2) + "|" + str(role) + "|" + str((servo1 + servo2 + role) / 3) + "#")
        arduino_veri_oku()
        time.sleep(0.1)

arduinoT = threading.Thread(target=arduino_thread, daemon=True)
arduinoT.start()

isKey = False
speed = 400
while isManual:
    isKey = False
    if keyboard.is_pressed("up"):
        servo1 = -speed
        isKey = True
        
    if keyboard.is_pressed("down"):
        servo1 = speed
        isKey = True
        
    if keyboard.is_pressed("left"):
        servo2 = speed
        isKey = True
        
    if keyboard.is_pressed("right"):
        servo2 = -speed
        isKey = True

    if(not isKey):
        servo1 = 0
        servo2 = 0
        role = 0
        



while not isManual:
    ret, frame = cap.read()
    if not ret:
        isRun = False
        servo1 = 0
        servo2 = 0
        role = 0
        arduino_veri_gonder(str(0) + "|" + str(0) + "|" + str(0) + "|" + str(0) + "#")
        print("Kamera görüntüsü alınamadı.")
        break
    
    #frame = cv2.flip(frame, 0)
    #frame = cv2.flip(frame, 1)


    camWith = frame.shape[1]
    camHeight = frame.shape[0]

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    #mask_1 = cv2.inRange(hsv, lower_red1, upper_red1)
    #mask_2 = cv2.inRange(hsv, lower_red2, upper_red2)

    #mask = mask_1 + mask_2

    #mask = cv2.inRange(hsv, lower_blue, upper_blue)

    mask = cv2.inRange(hsv, lower_green, upper_green)

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    targetList = []
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if(area > 10):
            cv2.drawContours(frame, [cnt], -1, (0, 255, 0), 2)
            x, y, w, h = cv2.boundingRect(cnt)
            targetList.append((area, x, y, w, h))

    if(len(targetList) > 0):
        targetList = sorted(targetList, key=lambda x: x[0], reverse=True)

        x = targetList[0][1]
        y = targetList[0][2]
        w = targetList[0][3]
        h = targetList[0][4]

        pixelX = -((camWith/2)-(x+w/2))
        pixelY = ((camHeight/2)-(y+h/2))

        if(pixelY > 100 and pixelX < -150):
            pixelY = 0
            pixelX = 0

            if(len(targetList) > 1):
                x = targetList[1][1]
                y = targetList[1][2]
                w = targetList[1][3]
                h = targetList[1][4]

                pixelX = -((camWith/2)-(x+w/2))
                pixelY = ((camHeight/2)-(y+h/2))
            
        t_now = time.time()

        dt = t_now - t_prev

        vx = (pixelX - x_prev) / dt #step motor hızını da çıkararak bul
        vy = (pixelY - y_prev) / dt

        x_prev = pixelX
        y_prev = pixelY

        x_future = pixelX + (vx * 0)
        y_future = pixelY + (vy * 0)

        servo2 = pidX.update(0, x_future)
        servo1 = pidY.update(0, y_future)

        distance = math.sqrt(pixelX**2 + pixelY**2)

        if(distance <= 20): #1 saniye boyunca kaldıysa ates et
            if(not isFire):
                fireTime = time.time()
                isFire = True
        else:
            isFire = False
            fireTime = time.time()
        
        role = 0
        if(isFire):
            if(time.time() - fireTime > 1):
                role = 1
            

        cv2.circle(frame, (int(camWith/2 + pixelX), int(camHeight/2 - pixelY)), 4, (255,255,0), -1)
        cv2.circle(frame, (int(camWith/2 + x_future), int(camHeight/2 - y_future)), 4, (0,0,255), -1)
        cv2.putText(frame, f"Find: {pixelX}, {pixelY}", (20, 110), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 4)
        cv2.putText(frame, f"speed: {vx:.2}, {vy:.2}", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 4)
    else:
        pixelX = 0
        pixelY = 0
        
        role = 0
        
        servo1 = 0
        servo2 = 0

        pidX.reset()
        pidY.reset()
    
    cv2.circle(frame, (int(camWith/2), int(camHeight/2)), 20, (0, 0, 255), 2)
    cv2.circle(frame, (int(camWith/2), int(camHeight/2)), 4, (255, 0, 0), -1)

    cv2.imshow("Kamera", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

isRun = False
arduino_veri_gonder(str(0) + "|" + str(0) + "|" + str(0) + "|" + str(0) + "#")
cap.release()
cv2.destroyAllWindows()
