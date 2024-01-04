//nextion masih error data tidak muncul
//nilai pitch_mm apakah sama dengan shoedistance ?
#include "EasyNextionLibrary.h"

#define encoderA 2  //Output A from rotary encoder,CW maka LOW TO HIGH, CCW maka HIGH TO LOW
#define encoderB 3  //Output B from rotary encoder,CW maka LOW TO HIGH, CCW maka HIGH TO LOW

int     dist_mm	    = 10;     // Dari spek autonics, jarak tiap pulse
int     circ_mm	    = 250;    // Dari spek autonics, keliling roda
int     ppr		      = 100;    // Dari spek autonics, pulse tiap putaran
int     ratio		    = 4;      // Dari spek autonics, perbandingan gear
int     loop_ms	    = 1000;   // Mau seberapa lama tiap loop nya?

int     pulses      = 0;      // Inisialisasi pulse untuk wadah counter

int     pitch_mm    = 330;      // Jarak antar garis kuning sepatu yang ada di conveyor

int     speed_std   = 0;
float   speed       = 0;      // Inisialiasi speed (mm/s)
int     speed_i     = 0;      // Inisialisasi speed (mm/s) versi integer

float   rate_min    = 7;
int     rate_min_i  = 0;
String  rate_min_s  = "";

float   rate_max    = 100;
int     rate_max_i  = 0;
String  rate_max_s  = "";

float   rate_act    = 0;
int     rate_act_i  = 0;

String  msg_simpan = "Data tersimpan"; 

float dpp = (dist_mm * ratio);  // distance per pulse  

unsigned long time_last = 0;    // markah atau penanda waktu terakhir loop sebelumnya di eksekusi

EasyNex myNex(Serial);

void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), counter_2, RISING);      // cw (kabel ke kanan) ccw (kabel ke kiri)
  // attachInterrupt(digitalPinToInterrupt(encoderB), counter_3, RISING);
}

void loop() {
  myNex.NextionListen();
  readnex();
  if (millis() - time_last >= loop_ms) {
    speed = (dpp * 1 / ppr * pulses) / (loop_ms / 1000);
    Serial.print ("Block loop speed: ");
    Serial.println (speed);
    if (speed <= 0) {
      rate_act = 0;
    } else {
      rate_act = pitch_mm / speed;
    }
    readnex();
    writenex();
    pulses = 0;
    Serial.println(String(rate_min_s) + "," + String(rate_max_s) + "," + String(rate_act) + "," + String(pulses));
    time_last = millis();
  }
}

void counter_2() {
  if (digitalRead(3) == HIGH) {
    pulses++;
    Serial.print ("Block counter_2: ");
    Serial.println (pulses);
  } else {
    pulses = pulses;
  }
}

void counter_3() {
  if (digitalRead(2) == HIGH) {
    pulses++;
    Serial.println("Block counter_3:" );
    Serial.println (pulses);
  } else {
    pulses = pulses;
  }
}
