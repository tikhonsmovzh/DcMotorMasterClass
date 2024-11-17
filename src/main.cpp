#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"
#include "VelEstimator.h"

void setup() {
  Serial.begin(9600);

  encsInit();
}

void loop() {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us);
    timer = micros();

    leftEncTick();
    rightEncTick();

    velocityTick();

    Serial.print("right: vel = ");
    Serial.print(gRightW);
    Serial.print(" pos = ");
    Serial.print(gRightHeading);
    Serial.print(" left: vel = ");
    Serial.print(gLeftW);
    Serial.print(" pos = ");
    Serial.println(gLeftHeading);
}