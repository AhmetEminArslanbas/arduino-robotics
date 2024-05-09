

void sensoroku(){

unsigned int position = qtra.readLine(sensors,1,zemin);// 7  6  5 4 3 2 1 0
hata=position-3500;
//if (yol_ayrimi_say==3)
//{
//if(sensors[0]<200&&sensors[7]<200 && (sensors[3]>700||sensors[4]>700)){zemin=0;} // beyaz
//if(sensors[0]>700&&sensors[7]>700 && (sensors[3]<200||sensors[4]<200)){zemin=1;} // siyah
//}
}
