#include <Arduino.h>
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"
#include "FunctionSwitcher.h"
#include "Voltmeter.h"
#include "Motor.h"
#include "DriveTrain.h"
#include "Cyclograms.h"

void setup() {
  Serial.begin(9600);

  encsInit();
  functionInit();
  voltInit();
  motorInit();
  cyclogramsInit();
}

void loop() {
  static uint32_t timer = micros();
  while(micros() - timer < Ts_us);
  timer = micros();

  encsTick();
  odometryTick();
  velocityTick();
  functionTick();
  voltTick();
  motorTick();

  cyclogramsTick();
} 