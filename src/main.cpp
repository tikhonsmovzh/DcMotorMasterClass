#include <Arduino.h>
#include "Encoder.h"
#include "VelEstimator.h"
#include "Odometry.h"
#include "FunctionSwitcher.h"
#include "Voltmeter.h"
#include "Motor.h"
#include "DriveTrain.h"
#include "Cyclograms.h"
#include "Maze.h"
#include "Vec2Int.h"
#include "MazeDrawer.h"
#include "Mazes.h"
#include "DistanceSensor.h"
#include "MazeSolver.h"

Maze maze = Maze();
MazeSolver solver = MazeSolver();

void setup()
{
  Serial.begin(9600);

  encsInit();
  functionInit();
  voltInit();
  motorInit();
  cyclogramsInit();
  distanceSensorsInit();

  maze2(&maze);

  solver.findPath(Vec2Int(0, 0), Vec2Int(9, 9), &maze);

  //drawMaze(&maze, &solver);
}

void loop()
{
  static uint32_t timer = micros();
  while (micros() - timer < Ts_us)
    ;
  timer = micros();

  encsTick();
  odometryTick();
  velocityTick();
  functionTick();
  voltTick();
  motorTick();
  distanceSensorsTick();

  cyclogramsTick();
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