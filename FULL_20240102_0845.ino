#include "EasyNextionLibrary.h"

const int  encoderA = 2; //Output A from rotary encoder
const int  encoderB = 3; //Output B from rotary encoder

float pulses = 0;
float rpm = 0;
float rps = 0;

float pulsesperturn = 2048; //pulsa yang dibutuhkan untuk menyelesaikan 1 putaran
float wheel_diameter = 70; //70 apa ?

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
String save = "Data Save Succesfully";

unsigned long timeold = 0;
unsigned long senddata_py = 0;
unsigned long lastDebounceTime[2] = {0, 0};  // Separate for each encoder pin
unsigned long debounceDelay = 50;            // Adjust delay as needed

EasyNex myNex(Serial2);

void setup() {
  Serial.begin(9600);
  myNex.begin(9600);
  pinMode(encoderA, INPUT_PULLUP);  // Pin Encoder A to pin 2 Arduino
  pinMode(encoderB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), counter, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderB), counter1, RISING);

  Serial.print("Pulses/sec ");
  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.print("Velocity[mm/s] ");
  Serial.println("Shoepairs ");
}

void loop() {
  myNex.NextionListen();
  readnex();
  //    //====================================================================
  if (millis() - timeold >= 5000) {
    rps = pulses / pulsesperturn;
    rpm = rps * 60;
    
    // velocity = (rps * wheel_diameter * PI) / 60;  // No need for ceil()
    // velocity = (PI * wheel_diameter) * rps / 60;
     velocity = (2 * PI * wheel_diameter) * rps / 60; // (keliling = 2 * PI * r)
    
    if (velocity <= 0) {
      shoepairs = 0;
    } else {
      shoepairs = shoedistance / velocity;
    }
    //====================================================================
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
    //pulses;
    timeold = millis();
  }
  //====================================================================
  if (millis() - senddata_py >= 5000) {
    Serial.println(String(rpm) + "," + String(pulses) + "," + String(velocity) + "," + String(shoepairs));
    senddata_py = millis();
  }
  //====================================================================
}
void counter() {
  int pinB = digitalRead(encoderB);
  if (pinB == HIGH && millis() - lastDebounceTime[0] >= debounceDelay) {
    pulses++;
    lastDebounceTime[0] = millis();
  }
  else if (!pinB) {
    pulses--;
  }
}
  void counter1() {
    int pinA = digitalRead(encoderA);
    if (pinA == HIGH && millis() - lastDebounceTime[1] >= debounceDelay) {
      pulses++;
      lastDebounceTime[1] = millis();
    }
  else if (!pinA) {
    pulses--;
  }
}
