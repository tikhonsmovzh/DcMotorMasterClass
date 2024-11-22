#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"

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
    odometryTick();
    velocityTick();

    Serial.print(gRobotState.x * 100);
    Serial.print(" ");
    Serial.print(gRobotState.y * 100);
    Serial.print(" ");
    Serial.println(gRobotState.h / PI * 180);
}