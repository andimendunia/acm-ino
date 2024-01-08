#include <Encoder.h>

// Deklarasi Pin Encoder
#define encoderA 2  //output A rotary encoder
#define encoderB 3  //output B rotary encoder

const float circ_mm   = 250.0;   //Dari spek autonics, keliling roda dalam milimeter
const int ppr         = 100;           //Dari spek autonics, pulse tiap putaran
const float pitch_mm  = 330.0;  //Jarak antar garis kuning sepatu yang ada di conveyor

float rps             = 0;
float speed           = 0;
float rate_act        = 0;
unsigned long time_last = 0;

// Deklarasi objek encoder
Encoder myEncoder(encoderA, encoderB);

void setup() {
  Serial.begin(9600);
  time_last = 0;
  delay(1000);  // Tambahkan delay untuk memastikan Serial Monitor siap
}

void loop() {
  if (millis() - time_last >= 1000) {
    // Dummy baca pulse
    // long pulses = 132;

    // Baca nilai pulsa dari encoder
    long pulses = myEncoder.read();

    rps = pulses / ppr;
    speed = round(rps * circ_mm);
    if (speed <= 0) {
      rate_act = 0;
    } else {
      rate_act = round(pitch_mm / speed);
    }
    // Hitung jarak berdasarkan jumlah pulsa
    float distance = (pulses * circ_mm) / ppr;

    // Tampilkan informasi pada Serial Monitor
    Serial.print("Pulse: ");
    Serial.print(pulses);
    // Serial.print(" | distance: ");
    // Serial.print(distance);
    Serial.print(" | Speed: ");
    Serial.print(speed);
    Serial.print(" mm/s| rate_act: ");
    Serial.print(rate_act);
    Serial.println(" sec/prs ");
    // Tunggu sejenak sebelum membaca ulang
    time_last = millis();
  }
}