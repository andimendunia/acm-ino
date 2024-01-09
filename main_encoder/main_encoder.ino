#include <Encoder.h>
#include "EasyNextionLibrary.h"

// Deklarasi Pin Encoder
#define encoderA 2  // output A rotary encoder
#define encoderB 3  // output B rotary encoder

// Pengaturan encoder
Encoder myEncoder(encoderA, encoderB);  // ambil dari definisi di atas
const int ppr = 2048 * 4;                    // pulse per revolution
const float circ = 220;                 // dalam mm

// Variabel untuk hitung pulsa
const int duration = 5000;  // Durasi (ms) untuk satu siklus
long pulseCount = 0;
unsigned long lastTime = 0;

// Variabel untuk hitung speed dan rate
int pitch = 330;  // Jarak (mm) antar garis kuning sepatu yang ada di conveyor

int speedStd = 0;
float speed = 0;   // Inisialiasi speed (mm/s)
int speedInt = 0;  // Inisialisasi speed (mm/s) versi integer

float rateMin = 7;
int rateMinInt = 0;
String rateMinStr = "";

float rateMax = 100;
int rateMaxInt = 0;
String rateMaxStr = "";

float rateAct = 0;
int rateActInt = 0;

const String msg_simpan = "Data tersimpan";
EasyNex myNex(Serial2);  //RX16,TX17

void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  delay(1000);
}

void loop() {
  myNex.NextionListen();
  readnex();
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= duration) {  // Jika sudah lebih dari durasi yang ditentukan
    lastTime = currentTime;

    // Baca pulsa menggunakan library si Paul
    pulseCount = abs(myEncoder.read());

    // Serial.print("Pulse count: ");
    // Serial.print(pulseCount);

    // Hitung speed (jarak (pulseCount / ppmm) bagi waktu (duration ms jadi s))
    float ppmm = (ppr / circ);
    speed = (pulseCount / ppmm) / (duration / 1000);
    rateAct = speed > 0 ? (pitch / speed) : 0;
    
    //serial monitor
    // Serial.print(" | Speed (mm/s): ");
    // Serial.print(speed);

    // Serial.print(" | Rate (sec/prs): ");
    // Serial.println(rateAct);

    readnex();
    writenex();
    Serial.println(String(rateMinInt) + "," + String(rateMaxInt) + "," + String(rateActInt) + "," + String(pulseCount));
    myEncoder.write(0);  // Kembalikan pulsa untuk siklus selanjutnya
  }
}
