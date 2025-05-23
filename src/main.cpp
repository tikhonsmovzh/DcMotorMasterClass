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

void setup()
{
  Serial.begin(9600);

  pinMode(ADDITION_LED_PIN, OUTPUT);

  digitalWrite(ADDITION_LED_PIN, 0);

  motorInit();
  functionInit();

  setLeftU(0.0f);
  setRightU(0.0f);

  do{
    functionTick();
  }
  while (gCurrentFunction != 16);

  delay(1000);
  
  digitalWrite(ADDITION_LED_PIN, 1);

  encsInit();
  voltInit();
  cyclogramsInit();
  distanceSensorsInit();
  mazeExplorerInit();
  odometryInit();

  addCyclogramToQueue(new Start());
  // addCyclogramToQueue(new Rotate90(true));
  // addCyclogramToQueue(new Rotate180());
  // addCyclogramToQueue(new Forward(false));
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

  mazeExplorerTick();
  cyclogramsTick();

  // Serial.print("Left Front: ");
  // Serial.print(gDistanceFrontLeft);
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