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

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void mazeExplorerInit()
{
    _maze.set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
}

Vec2Int _currentRobotPos(0, 1);
Direction _currentRobotDirection = DOWN;

Maze::Cell rotateCell(Maze::Cell cell, Direction dir){
    switch (dir)
    {
    case UP:
        return cell;

    case DOWN:
    {
        Maze::Cell resultCell;

        resultCell.up = cell.down;
        resultCell.down = cell.up;
        resultCell.left = cell.right;
        resultCell.right = cell.left;

        return resultCell;
    }

    case LEFT:
    {
        Maze::Cell resultCell;

        resultCell.up = cell.right;
        resultCell.down = cell.left;
        resultCell.left = cell.up;
        resultCell.right = cell.down;

        return resultCell;
    }

    case RIGHT:
    {
        Maze::Cell resultCell;

        resultCell.up = cell.left;
        resultCell.down = cell.right;
        resultCell.left = cell.down;
        resultCell.right = cell.up;

        return resultCell;
    }
    }

    return cell;
}

void moveToForward()
{
    runCyclogram(new Forward());

    switch (_currentRobotDirection)
    {
    case UP:
        _currentRobotPos.y--;
        break;

    case DOWN:
        _currentRobotPos.y++;
        break;

    case LEFT:
        _currentRobotPos.x--;
        break;

    case RIGHT:
        _currentRobotPos.x++;
        break;
    }
}

void moveToLeft()
{
    runCyclogram(new Rotate90(true));

    switch (_currentRobotDirection)
    {
    case UP:
        _currentRobotPos.x--;
        _currentRobotDirection = LEFT;
        break;

    case DOWN:
        _currentRobotPos.x++;
        _currentRobotDirection = RIGHT;
        break;

    case LEFT:
        _currentRobotPos.y++;
        _currentRobotDirection = DOWN;
        break;

    case RIGHT:
        _currentRobotPos.y--;
        _currentRobotDirection = UP;
        break;
    }
}

void moveToRight()
{
    runCyclogram(new Rotate90(false));

    switch (_currentRobotDirection)
    {
    case UP:
        _currentRobotPos.x++;
        _currentRobotDirection = RIGHT;
        break;

    case DOWN:
        _currentRobotPos.x--;
        _currentRobotDirection = LEFT;
        break;

    case LEFT:
        _currentRobotPos.y--;
        _currentRobotDirection = UP;
        break;

    case RIGHT:
        _currentRobotPos.y++;
        _currentRobotDirection = DOWN;
        break;
    }
}

void moveToRevers()
{
    runCyclogram(new Rotate180());

    switch (_currentRobotDirection)
    {
    case UP:
        _currentRobotPos.y++;
        _currentRobotDirection = DOWN;
        break;

    case DOWN:
        _currentRobotPos.y--;
        _currentRobotDirection = UP;
        break;

    case LEFT:
        _currentRobotPos.x--;
        _currentRobotDirection = RIGHT;
        break;

    case RIGHT:
        _currentRobotPos.x++;
        _currentRobotDirection = LEFT;
        break;
    }
}

void mazeExplorerTick()
{
    if (!isCyclogramsEmpty())
        return;

    if(!isWallLeft())
        runCyclogram(new Rotate90(true));
    else if(!isWallForward())
        runCyclogram(new Forward());
    else if(!isWallRight())
        runCyclogram(new Rotate90(false));
    else
        runCyclogram(new Rotate180());

    // Maze::Cell currentCell = rotateCell(_maze.get(_currentRobotPos), _currentRobotDirection);

    // if (isWallForward())
    //     currentCell.up = Maze::WALL;
    // else
    //     currentCell.up = Maze::EMPTY;

    // if (isWallLeft())
    //     currentCell.left = Maze::WALL;
    // else
    //     currentCell.left = Maze::EMPTY;

    // if (isWallRight())
    //     currentCell.right = Maze::WALL;
    // else
    //     currentCell.right = Maze::EMPTY;

    // _maze.set(rotateCell(currentCell, _currentRobotDirection), _currentRobotPos);

    // _solver.findPath(_currentRobotPos, Vec2Int(CELL_END_X, CELL_END_Y), &_maze);

    // MazeSolver::CellState state = _solver.getCellState(_currentRobotPos);

    // switch (_currentRobotDirection)
    // {
    // case UP:
    //     if (state == MazeSolver::UP)
    //         moveToForward();
    //     else if (state == MazeSolver::LEFT)
    //         moveToLeft();
    //     else if (state == MazeSolver::RIGHT)
    //         moveToRight();
    //     else
    //         moveToRevers();

    //     break;

    // case LEFT:
    //     if (state == MazeSolver::UP)
    //         moveToRight();
    //     else if (state == MazeSolver::LEFT)
    //         moveToForward();
    //     else if (state == MazeSolver::DOWN)
    //         moveToLeft();
    //     else
    //         moveToRevers();

    //     break;

    // case RIGHT:
    //     static int counter = 0;

    //     counter++;

    //     if(counter > 3){
    //                     setLeftU(0.0f);
    //         setRightU(0.0f);
    //         drawMaze(&_maze, &_solver);

    //         delay(999999);
    //     }

    //     if (state == MazeSolver::UP)
    //         moveToLeft();
    //     else if (state == MazeSolver::RIGHT)
    //         moveToForward();
    //     else if (state == MazeSolver::DOWN)
    //         moveToRight();
    //     else
    //         moveToRevers();

    //     break;

    // case DOWN:
    //     if (state == MazeSolver::LEFT)
    //         moveToRight();
    //     else if (state == MazeSolver::DOWN)
    //         moveToForward();
    //     else if (state == MazeSolver::RIGHT)
    //         moveToLeft();
    //     else
    //         moveToRevers();

    //     break;
    // }
}