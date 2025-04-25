#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Maze.h"
#include "MazeDrawer.h"
#include "MazeSolver.h"
#include "Vec2Int.h"
#include "Cyclograms.h"
#include "DistanceSensor.h"
#include "Odometry.h"

Maze _maze = Maze();
MazeSolver _solver = MazeSolver();

void mazeExplorerInit()
{

}

Maze::Cell rotateCell(float h, Maze::Cell cell){
    Maze::Cell rotatedCell;

    if(abs(h) < H_SENS){
        rotatedCell.up = cell.up;
        rotatedCell.down = cell.down;
        rotatedCell.left = cell.left;
        rotatedCell.right = cell.right;
    } else if(abs(h - PI * 0.5f) < H_SENS){
        rotatedCell.left = cell.up;
        rotatedCell.right = cell.down;
        rotatedCell.down = cell.left;
        rotatedCell.up = cell.right;
    } else if(abs(h - PI) < H_SENS){
        rotatedCell.up = cell.down;
        rotatedCell.down = cell.up;
        rotatedCell.left = cell.right;
        rotatedCell.right = cell.left;
    } else {
        rotatedCell.left = cell.down;
        rotatedCell.right = cell.up;
        rotatedCell.down = cell.right;
        rotatedCell.up = cell.left;
    }

    return rotatedCell;
}

uint32_t _lastCycTime = 0;

void mazeExplorerTick()
{
    if(!isCyclogramsEmpty()){
        _lastCycTime = millis();

        return;
    }

    if(millis() - _lastCycTime < CYCLOGRAM_DELAY)
        return;

    // Serial.print(gRobotState.x);
    // Serial.print(" ");
    // Serial.print(gRobotState.y);
    // Serial.println();

    Maze::Cell nextCell;

    if(isWallForward())
        nextCell.up = Maze::WALL;
    else
        nextCell.up = Maze::EMPTY;

    if(isWallLeft())
        nextCell.left = Maze::WALL;
    else
        nextCell.left = Maze::EMPTY;

    if(isWallRight())
        nextCell.right = Maze::WALL;
    else
        nextCell.right = Maze::EMPTY;

    if(nextCell.left == Maze::EMPTY)
        runCyclogram(new Rotate90(true));
    else if(nextCell.up == Maze::EMPTY)
        runCyclogram(new Forward());
    else if(nextCell.right == Maze::EMPTY)
        runCyclogram(new Rotate90(false));
    else
        runCyclogram(new Rotate180());

    Maze::Cell rotatedNextCell;
    
    Vec2Int robotPosition(gRobotState.x / CELL_SIZE, gRobotState.y / CELL_SIZE);

    rotatedNextCell = rotateCell(gRobotState.h, nextCell);

    if(abs(gRobotState.h) < H_SENS)
        robotPosition.y++;
    else if(abs(gRobotState.h - PI * 0.5f) < H_SENS)
        robotPosition.x++;
    else if(abs(gRobotState.h - PI) < H_SENS)
        robotPosition.y--;
    else
        robotPosition.x--;

    Maze::Cell mazeCell = _maze.get(robotPosition);

    if(rotatedNextCell.up == Maze::UNKNOWN)
        rotatedNextCell.up = mazeCell.up;

    if(rotatedNextCell.down == Maze::UNKNOWN)
        rotatedNextCell.down = mazeCell.down;
        
    if(rotatedNextCell.left == Maze::UNKNOWN)
        rotatedNextCell.left = mazeCell.left;
    
    if(rotatedNextCell.right == Maze::UNKNOWN)
        rotatedNextCell.right = mazeCell.right;

    _maze.set(rotatedNextCell, robotPosition);
}