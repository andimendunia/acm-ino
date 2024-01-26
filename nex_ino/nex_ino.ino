#include <Encoder.h>                                //library encoder
#include <EasyNextionLibrary.h>                     //library nextion
#include <EEPROM.h>

// Deklarasi Pin Encoder
#define encoderA 2                                  //output A rotary encoder
#define encoderB 3                                  //output B rotary encoder

// Pengaturan encoder
Encoder myEncoder(encoderA, encoderB);              //ambil dari definisi di atas

//Hitung PPMM
const int ppr                = 2048 * 4;            //pulse per revolution encoder ASM
const float circ             = 220;                 //dalam mm

//Hitung Speed
float speed                  = 0;                  //Inisialiasi speed (mm/s)
long pulseCount              = 0;
const int duration           = 5000;               //Durasi (ms) untuk satu siklus  
unsigned long lastTime       = 0;

//Hitung rateAct
float rateAct                = 0;
int pitch                    = 330;                //Jarak (mm) antar garis kuning sepatu yang ada di conveyor

//Read dari Nextion ke Arduino
String rateMinStr;
String rateMaxStr;

//Write dari Arduino ke Nextion
int speedInt                 = 0;                  //Inisialisasi speed (mm/s) versi integer
int rateActInt               = 0;
int speedStd                 = 0;
int rateMinInt               = 0;
int rateMaxInt               = 0;

EasyNex myNex(Serial3);//RX15,TX14

void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  delay(1000);
}

void loop() {
  myNex.NextionListen();
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= duration) {  // Jika sudah lebih dari durasi yang ditentukan
    lastTime = currentTime;

    // Baca pulsa menggunakan library si Paul
    // pulseCount = abs(myEncoder.read());
      pulseCount  = 8924; //dummy

    // Hitung speed (jarak (pulseCount / ppmm) bagi waktu (duration ms jadi s))
    float ppmm    = (ppr / circ);
    speed         = (pulseCount / ppmm) / (duration / 1000);
    rateAct       = speed > 0 ? (pitch / speed) : 0;

    nexcom();
    Serial.println(String(rateMinInt) + "," + String(rateMaxInt) + "," + String(rateActInt) + "," + String(pulseCount)); 
    myEncoder.write(0);  // Kembalikan pulsa untuk siklus selanjutnya
  }
}

//buat kondisi awal t1,t5 ngirim angka minimal yang awalnya dari nextion (berarti yg rubah t9+t10)
void nexcom() {
  rateMinStr = myNex.readStr("rateMin.txt");
  rateMaxStr = myNex.readStr("rateMax.txt");
   if (((rateMinStr == "")) && (rateMaxStr == "")) {
    rateMinInt = 7;   //untuk batas min eeprom
    rateMaxInt = 10;  //untuk batas max eeprom
    myNex.writeStr("rateMin.txt", String(rateMinInt));
    myNex.writeStr("rateMax.txt", String(rateMaxInt));

  } else {
    rateMinInt = rateMinStr.toFloat();
    rateMaxInt = rateMaxStr.toFloat();
  }

  speedInt    = speed;
  rateActInt  = ceil(rateAct); // ceil bulat ke atas
  speedStd    = pitch / rateMinInt;

  myNex.writeStr("speedStd.txt", String(speedStd));
  
  myNex.writeStr("speed.txt", String(speedInt));
  myNex.writeStr("rateActRound.txt", String(rateActInt)); 
  myNex.writeStr("pulse.txt", String(pulseCount));

  if (rateActInt >= rateMinInt && rateActInt <= rateMaxInt) {
    myNex.writeNum("t2.pic", 3);//hijau
    myNex.writeNum("t3.pic", 4);
  } else {
    myNex.writeNum("t2.pic", 5);
    myNex.writeNum("t3.pic", 6);//warna merah
  }
}