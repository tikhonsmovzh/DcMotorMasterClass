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

void setup()
{
  delay(1000);

  Serial.begin(9600);

  encsInit();
  functionInit();
  voltInit();
  motorInit();
  cyclogramsInit();
  distanceSensorsInit();
  mazeExplorerInit();

  addCyclogramToQueue(new Forward(true));
  // addCyclogramToQueue(new Rotate90(true));
  // addCyclogramToQueue(new Rotate90(true));
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

  cyclogramsTick();
  mazeExplorerTick();
  
  // Serial.print("Left Front: ");
  // Serial.print(gDistanceFrontLeft);
  // Serial.print("  Right Front: ");
  // Serial.print(gDistanceFrontRight);
  // Serial.print("  Left diagonal: ");
  // Serial.println(gDistanceDiagonalLeft);
  // Serial.print("  Right diagonal: ");
  // Serial.print(gDistanceDiagonalRight);
  // Serial.println();
}