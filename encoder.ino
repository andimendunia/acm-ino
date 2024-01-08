#include <Encoder.h>

// Deklarasi Pin Encoder
#define encoderA 2 //output A rotary encoder
#define encoderB 3 //output B rotary encoder

const float circ_mm   = 220.0; //Dari spek autonics, keliling roda dalam milimeter
const int   ppr       = 2048;  //Dari spek autonics, pulse tiap putaran
const float pitch_mm  = 330.0; //Jarak antar garis kuning sepatu yang ada di conveyor

// Deklarasi objek encoder
Encoder myEncoder(encoderA, encoderB);

// Deklarasi awal pulse dibaca dari 0
volatile long pulses  = 0;

// Callback function
void callback() {
  pulses  = 0;
}

// Encoder interrupts ketika mencapai nilai pulsa = 3072
void interrupts_encoder() {
  pulses++;
  if (pulses == 3072) {
    callback();
  }
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), interrupts_encoder, RISING);
  delay(1000); // Tambahkan delay untuk memastikan Serial Monitor siap
}

void loop() {
  // Baca nilai pulsa dari encoder
  //  long pulses = myEncoder.read();
  // Dummy baca pulse
  long pulses = 3072;

  float distance = (pulses * circ_mm) / ppr;

  //  // Hitung kecepatan conveyor
  //  float speed = pulses * circ_mm / (ppr * pitch_mm);

  // Tampilkan informasi pada Serial Monitor
  Serial.print("Pulse: ");
  Serial.print(pulses);
  Serial.print(" | Speed: ");
  //  Serial.print(speed);
  //  Serial.print("mm/s");
  Serial.print(" | distance: ");
  Serial.print(distance);
  Serial.println(" mm");

  // Tunggu sejenak sebelum membaca ulang
  delay(1000);
}
