#include "EasyNextionLibrary.h"

#define encoderA 2
#define encoderB 3

float pulses = 0;
// float pulses_1 = 0;
float rpm = 0;
float rps = 0;
float velocity = 0;
int fix_velocity = 0;
unsigned long timeold = 0;
// unsigned long timeold_1 = 0;
float pulsesperturn = 2048;
// float pulsesperturn = 16384;
float wheel_diameter = 70;
float shoepairs = 0;
int fix_shoepairs = 0;
int shoedistance = 330;
float standardup = 100;
int fix_standarddown = 0;
float standarddown = 7;
float standardspeed = 0;
int fix_standardspeed = 0;
String str_up = "0";
String str_down = "0";
String save = "Data Save Succesfully";

unsigned long lastDebounceTime[2] = {0, 0};  // Separate for each encoder pin
unsigned long debounceDelay = 50;            // Adjust delay as needed

//=======================================
// kirim data ke python

// String rate_min;
// String rate_max;
// String rate_actual;

//=======================================


static volatile unsigned long debounce = 0;
static volatile unsigned long debounce1 = 0;

EasyNex myNex(Serial2);


void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  pinMode(encoderA, INPUT_PULLUP);  // Pin Encoder A to pin 2 Arduino
  pinMode(encoderB, INPUT_PULLUP);
  pulses = 0;
  // pulses_1 = 0;
  rpm = 0;
  timeold = 0;
  // timeold_1 = 0;

  Serial.print("Pulses/sec ");
  Serial.print("Seconds ");
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
  if (millis() - timeold >= 5000) {
    rps = pulses / pulsesperturn;
    // rpm = rps * 60;
    // velocity = (rps * wheel_diameter * PI) / 60;  // No need for ceil()
    velocity = (PI * wheel_diameter) * rps / 60;

    if (velocity <= 0) {
      shoepairs = 0;
    } else {
      shoepairs = shoedistance / velocity;
    }
    Serial.print(millis() / 5000);
    Serial.print("       ");
    Serial.print(rpm);
    Serial.print("   ");
    Serial.print(pulses);
    Serial.print("     ");
    Serial.print(velocity);
    Serial.print("           ");
    Serial.println(shoepairs);
    readnex();
    writenex();
    pulses = 0;
    timeold = millis();
  }

  //====================================================================
  // if (millis() - timeold >= 5000) {
  //   rate_min = (rpm);
  //   rate_max = (pulses);
  //   rate_actual = (velocity);

  //   Serial.println(rate_min + "," + rate_max + "," + rate_actual);
  //====================================================================
  // }
}

void counter() {
    if (digitalRead(3) == HIGH && millis() - lastDebounceTime[0] >= debounceDelay) {
        pulses++;
        lastDebounceTime[0] = millis();
    }
}

void counter1() {
    if (digitalRead(2) == HIGH && millis() - lastDebounceTime[1] >= debounceDelay) {
        pulses++;
        lastDebounceTime[1] = millis();
    }
}
