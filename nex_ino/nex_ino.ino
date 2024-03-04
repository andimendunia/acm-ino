#include <Encoder.h>                                //library encoder
#include <EasyNextionLibrary.h>                     //library nextion
//#include <SoftwareSerial.h>                       //library SoftwareSerial  

//// Deklarasi Pin TRANSMIT
//#define enTxPin 4                                 //HIGH: TX and LOW: RX (RE DE RS485 SOLDER)

// Deklarasi Pin Encoder
#define encoderA 2                                  //output A rotary encoder
#define encoderB 3                                  //output B rotary encoder

// Pengaturan encoder
Encoder myEncoder(encoderA, encoderB);              //ambil dari definisi di atas

// //Hitung PPMM
// const int ppr                = 2048 * 4;            //pulse per revolution encoder ASM
// const float circ             = 220;                 //dalam mm

//Hitung PPMM
const int ppr                = 100;                 //pulse per revolution encoder Autonic
const float circ             = 250;                 //dalam mm

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

//// Communication RS485 with arduino
//SoftwareSerial mySerial(19, 18); //RX19,TX18

// Communication nextion with arduino
//EasyNex myNex(Serial);  //RX0,TX0
//EasyNex myNex(Serial3);//RX15,TX14
// EasyNex myNex(Serial1);//RX19,TX18
 EasyNex myNex(Serial2);//RX17,TX16

void setup() {
  Serial.begin(9600);   // reenable serial again
  delay(500);
  while (!Serial) { 
    ; // wait for serial port to connect. Needed for native USB
  }
  myNex.begin(9600);  //kirim ke nextion
  delay(500); 
  //mySerial.begin(9600); //kirim ke RS485
  //pinMode       (enTxPin, OUTPUT);
  //digitalWrite  (enTxPin, HIGH);// default TX untuk transmit data
}

void loop() {
  myNex.NextionListen();
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= duration) {  // Jika sudah lebih dari durasi yang ditentukan
    lastTime = currentTime;

    // Baca pulsa menggunakan library si Paul
//    pulseCount = abs(myEncoder.read());
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

void nexcom() {
  rateMinStr = myNex.readStr("rateMin.txt");     //t9.txt dikirim ke t1.txt
  rateMaxStr = myNex.readStr("rateMax.txt");     //t10.txt dikirim ke t5.txt
  
   if (((rateMinStr == "")) && (rateMaxStr == "")) {
    rateMinInt = 7;   //untuk batas min eeprom
    rateMaxInt = 10;  //untuk batas max eeprom    
    myNex.writeStr("rateMin.txt", String(rateMinInt));  //t9.txt
    myNex.writeStr("rateMax.txt", String(rateMaxInt));  //t10.txt
  } else {
    rateMinInt = rateMinStr.toFloat();
    rateMaxInt = rateMaxStr.toFloat();
  }

  speedInt    = speed;
  rateActInt  = round(rateAct); // ceil bulat ke atas
  speedStd    = pitch / rateMinInt;

  myNex.writeStr("speedStd.txt", String(speedStd));             //t0.txt
  
  myNex.writeStr("speed.txt", String(speedInt));                //t2.txt
  myNex.writeStr("rateActRound.txt", String(rateActInt));       //t3.txt
  myNex.writeStr("pulse.txt", String(pulseCount));              //t4.txt

  if (rateActInt >= rateMinInt && rateActInt <= rateMaxInt) {
    myNex.writeNum("speed.pic", 3);//hijau
    myNex.writeNum("rateActRound.pic", 4);
  } else {
    myNex.writeNum("speed.pic", 5);
    myNex.writeNum("rateActRound.pic", 6);//warna merah
  }
}
