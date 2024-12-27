#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"
#include "FunctionSwitcher.h"
#include "Voltmeter.h"
#include "Motor.h"
#include "DriveTrain.h"

void setup() {
  Serial.begin(9600);

  encsInit();
  functionInit();
  voltInit();
  motorInit();
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
  motorTick();

  setDriveVelocity(0.2f, 5.74);
}