#include <Arduino.h>
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"
#include "FunctionSwitcher.h"
#include "Voltmeter.h"
#include "Motor.h"
#include "DriveTrain.h"
#include "Cyclograms.h"
#include "DistanceSensor.h"
#include "MazeExplorer.h"
#include "Config.h"
#include "SmartSolver.h"

void setup()
{
  Serial.begin(115200);

  delay(1000);

  functionInit();
  voltInit();
  encsInit();
  cyclogramsInit();
  distanceSensorsInit();
  mazeExplorerInit();
  odometryInit();
  motorInit();
}

void loop()
{
  static uint32_t timer = micros();
  while (micros() - timer < Ts_us);
  timer = micros();

  encsTick();
  odometryTick();
  velocityTick();
  functionTick();
  voltTick();
  motorTick();
  distanceSensorsTick();
  smartSolverTick();

  mazeExplorerTick();
  cyclogramsTick();

  // Serial.print("Left Front: ");
  // Serial.println(gDistanceFrontLeft);
  // Serial.print("  Right Front: ");
  // Serial.print(gDistanceFrontRight);
  // Serial.print("  Left diagonal: ");
  // Serial.print(gDistanceDiagonalLeft);
  // Serial.print("  Right diagonal: ");
  // Serial.print(gDistanceDiagonalRight);
  // Serial.println();

  // Serial.println(gVoltmeterVolts);

  // Serial.print("  forward = ");
  // Serial.print(isWallForward());
  // Serial.print("  left = ");
  // Serial.print(isWallLeft());
  // Serial.print("  right = ");
  // Serial.println(isWallRight());
}