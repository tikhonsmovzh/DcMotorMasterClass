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

    Serial.print(gLeftW);
    Serial.print(" ");
    Serial.println(gRightW);
}