#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"
#include "FunctionSwitcher.h"
#include "Voltmeter.h"

void setup() {
  Serial.begin(9600);

  encsInit();
  functionInit();
  voltInit();
}

void loop() {
  static uint32_t timer = micros();
  while(micros() - timer < Ts_us);
  timer = micros();

  leftEncTick();
  rightEncTick(); 
  odometryTick();
  velocityTick();
  functionTick();
  voltTick();

  Serial.println(gVoltmeterVolts);
}