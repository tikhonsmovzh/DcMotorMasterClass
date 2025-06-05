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
#include "ElapsedTime.h"

Maze _maze = Maze();
MazeSolver _solver = MazeSolver();

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum RunState
{
    SERCH_FINISH,
    SERCH_END,
    FAST_RUN,
    WAIT
};

Vec2Int _currentRobotPos(0, 0);
Direction _currentRobotDirection = DOWN;

int fixCounter = 0;
ElapseTime _startTimer;

RunState _currentRunState = WAIT;

Vec2Int _endPoint(CELL_END_X, CELL_END_Y);

void resetState()
{
    if (gCurrentFunction == 8)
    {
        _currentRobotPos.x = 1;
        _currentRobotPos.y = 0;

        _currentRobotDirection = RIGHT;

        _maze.set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    }
    else
    {
        _currentRobotPos.x = 0;
        _currentRobotPos.y = 1;

        _currentRobotDirection = DOWN;

        _maze.set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    }

    _endPoint.x = CELL_END_X;
    _endPoint.y = CELL_END_Y;

    addCyclogramToQueue(Start);
}

void mazeExplorerInit()
{
}

Maze::Cell cellToGlobal(Maze::Cell cell, Direction dir)
{
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

Maze::Cell cellToLocal(Maze::Cell cell, Direction dir)
{
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

        resultCell.up = cell.left;
        resultCell.down = cell.right;
        resultCell.left = cell.down;
        resultCell.right = cell.up;

        return resultCell;
    }

    case RIGHT:
    {
        Maze::Cell resultCell;

        resultCell.up = cell.right;
        resultCell.down = cell.left;
        resultCell.left = cell.up;
        resultCell.right = cell.down;

        return resultCell;
    }
    }

    return cell;
}

void moveToForward()
{
    fixCounter = 0;

    addCyclogramToQueue(Forward);

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
    fixCounter++;

    if (fixCounter % 2 == 0)
        addCyclogramToQueue(Rotate90LeftFix);
    else
        addCyclogramToQueue(Rotate90Left);

    if (fixCounter % 4 == 0)
    {
        Maze::Cell currentCell = cellToLocal(_maze.get(_currentRobotPos), _currentRobotDirection);

        if (currentCell.right == Maze::WALL)
            addCyclogramToQueue(Start);
    }

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
    fixCounter++;

    if (fixCounter % 2 == 0)
        addCyclogramToQueue(Rotate90RightFix);
    else
        addCyclogramToQueue(Rotate90Right);

    if (fixCounter % 4 == 0)
    {
        Maze::Cell currentCell = cellToLocal(_maze.get(_currentRobotPos), _currentRobotDirection);

        if (currentCell.left == Maze::WALL)
            addCyclogramToQueue(Start);
    }

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
    fixCounter = 0;

    addCyclogramToQueue(Rotate180);

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
        _currentRobotPos.x++;
        _currentRobotDirection = RIGHT;
        break;

    case RIGHT:
        _currentRobotPos.x--;
        _currentRobotDirection = LEFT;
        break;
    }
}

void mazeExplorerTick()
{
    if (_currentRunState == WAIT)
    {
        static bool isStartButtonPresed = false;

        if (gStartButtonPresed)
        {
            _startTimer.reset();

            isStartButtonPresed = true;

            return;
        }
        else if (isStartButtonPresed && _startTimer.milliseconds() > START_TIMER)
        {
            isStartButtonPresed = false;

            _currentRunState = FAST_RUN;

            resetState();
        }
        else
            return;
    }

    if (!isCyclogramsEmpty())
        return;

    Maze::Cell currentCell = cellToLocal(_maze.get(_currentRobotPos), _currentRobotDirection);

    if (isWallForward())
        currentCell.up = Maze::WALL;
    else
        currentCell.up = Maze::EMPTY;

    if (isWallLeft())
        currentCell.left = Maze::WALL;
    else
        currentCell.left = Maze::EMPTY;

    if (isWallRight())
        currentCell.right = Maze::WALL;
    else
        currentCell.right = Maze::EMPTY;

    _maze.set(cellToGlobal(currentCell, _currentRobotDirection), _currentRobotPos);

    // setLeftU(0.0f);
    // setRightU(0.0f);

    // drawMaze(&_maze, &_solver);

    // Serial.print(_currentRobotPos.x);
    // Serial.print(" ");
    // Serial.println(_currentRobotPos.y);

    _solver.findPath(_currentRobotPos, _endPoint, &_maze);

    MazeSolver::CellState state = _solver.getCellState(_currentRobotPos);

    if (state == MazeSolver::END || state == MazeSolver::UKNNOWN)
    {
        if (_currentRunState == SERCH_FINISH || _currentRunState == FAST_RUN)
        {
            _endPoint.x = 0;
            _endPoint.y = 0;
            _currentRunState = SERCH_END;
        }
        else if (_currentRunState == SERCH_END)
            _currentRunState = WAIT;

        return;
    }

    switch (_currentRobotDirection)
    {
    case UP:
        if (state == MazeSolver::UP)
            moveToForward();
        else if (state == MazeSolver::LEFT)
            moveToLeft();
        else if (state == MazeSolver::RIGHT)
            moveToRight();
        else
            moveToRevers();

        break;

    case LEFT:
        if (state == MazeSolver::UP)
            moveToRight();
        else if (state == MazeSolver::LEFT)
            moveToForward();
        else if (state == MazeSolver::DOWN)
            moveToLeft();
        else
            moveToRevers();

        break;

    case RIGHT:
        if (state == MazeSolver::UP)
            moveToLeft();
        else if (state == MazeSolver::RIGHT)
            moveToForward();
        else if (state == MazeSolver::DOWN)
            moveToRight();
        else
            moveToRevers();

        break;

    case DOWN:
        if (state == MazeSolver::LEFT)
            moveToRight();
        else if (state == MazeSolver::DOWN)
            moveToForward();
        else if (state == MazeSolver::RIGHT)
            moveToLeft();
        else
            moveToRevers();

        break;
    }
}