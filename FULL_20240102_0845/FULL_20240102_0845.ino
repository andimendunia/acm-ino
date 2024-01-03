#include "EasyNextionLibrary.h"

#define encoderA 2 //Output A from rotary encoder,CW maka LOW TO HIGH, CCW maka HIGH TO LOW
#define encoderB 3 //Output B from rotary encoder,CW maka LOW TO HIGH, CCW maka HIGH TO LOW

float pulses = 0;
float rpm = 0;
float rps = 0;

float pulsesperturn = 4096; // pulsa yang dibutuhkan untuk menyelesaikan 1 putaran
float wheel_circumference_mm = 220; // 70 apa ?

float velocity = 0;
int fix_velocity = 0;

float shoepairs = 0;
int fix_shoepairs = 0;

float standarddown = 7;
int fix_standarddown = 0;

float standardspeed = 0;
int fix_standardspeed = 0;

float standardup = 100;
int shoedistance = 330;

String str_up = "0";
String str_down = "0";
String save = "Data tersimpan";

//===============================
unsigned long sampling_timeold;
const unsigned long interval_timeold = 5000;

unsigned long sampling_senddata_py;
const unsigned long interval_senddata_py = 5000;
//===============================

unsigned long lastDebounceTime[2] = {0, 0}; // Separate for each encoder pin
const unsigned long debounceDelay = 50; // Adjust delay as needed ms

EasyNex myNex(Serial);
//EasyNex myNex(Serial2);

void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);

// susunan yang mau di kirim ke python masih belum tau urutannya udah sama atau belum
//  Serial.print("Pulses/sec ");
//  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.print("Velocity[mm/s] ");
  Serial.println("Shoepairs ");

  attachInterrupt(digitalPinToInterrupt(encoderA), counter, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderB), counter1, RISING);
}

void loop() {
  myNex.NextionListen();
  readnex();
  unsigned long currentTime = millis();
  if (currentTime - sampling_timeold >= interval_timeold) {
    rps = pulses / pulsesperturn;
    rpm = rps * 60;
    velocity = ceil((rps * wheel_circumference_mm) / 5);

    if (velocity <= 0) {
      shoepairs = 0;
    } else {
      shoepairs = shoedistance / velocity;
    }
    readnex();
    writenex();
    sampling_timeold = currentTime;
  }
// susunan yang mau di kirim ke python masih belum tau urutannya udah sama atau belum
  if (currentTime - sampling_senddata_py >= interval_senddata_py) {
    Serial.println(String(rpm) + "," + String(pulses) + "," + String(velocity) + "," + String(shoepairs));
    sampling_senddata_py = currentTime;
  }
}

void counter() {
  int pinA = digitalRead(encoderA);
  if (pinA == HIGH && millis() - lastDebounceTime[0] >= debounceDelay) {
    pulses++;
    lastDebounceTime[0] = millis();
    Serial.println("DIproses di encoderA");
  } else {
    pulses = pulses;
  }
}

void counter1() {
  int pinB = digitalRead(encoderB);
  if (pinB == HIGH && millis() - lastDebounceTime[1] >= debounceDelay) {
    pulses++;
    lastDebounceTime[1] = millis();
    Serial.println("DIproses di encoderB");
    delay(5000);
  } else {
    pulses = pulses;
  }
}
