#include "TFT_eSPI.h"
TFT_eSPI tft;

int SENSOR = D0;
#define PIN_A0

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR, INPUT);

}

void loop() {
  int S = digitalRead(SENSOR);
  Serial.println(S);
  if(S == 0){
    Serial.println("Obstacle Detected");
  }
  else{
    Serial.println("Clear");
  }
}
