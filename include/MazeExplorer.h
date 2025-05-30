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

Vec2Int _currentRobotPos(0, 1);
Direction _currentRobotDirection = DOWN;

void mazeExplorerInit()
{
    _maze.set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::WALL), Vec2Int(0, 0));

    randomSeed(analogRead(0));

    if(gCurrentFunction == 8){
        _currentRobotPos.x = 1;
        _currentRobotPos.y = 0;

        _currentRobotDirection = RIGHT;
    }
}

Maze::Cell rotateCell(Maze::Cell cell, Direction dir)
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

void moveToForward()
{
    runCyclogram(Forward);

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
    runCyclogram(Rotate90Left);

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
    runCyclogram(Rotate90Right);

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
    runCyclogram(Rotate180);

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

bool isRunToBase = false;

void mazeExplorerTick()
{
    if (!isCyclogramsEmpty())
        return;

    if (!isRunToBase)
    {
        int leftOrRight = random(0, 2);

        if (leftOrRight == 0)
        {
            if (!isWallLeft())
                moveToLeft();
            else if (!isWallForward())
                moveToForward();
            else if (!isWallRight())
                moveToRight();
            else
                moveToRevers();
        }
        else
        {
            if (!isWallRight())
                moveToRight();
            else if (!isWallForward())
                moveToForward();
            else if (!isWallLeft())
                moveToLeft();
            else
                moveToRevers();
        }

        Maze::Cell currentCell = rotateCell(_maze.get(_currentRobotPos), _currentRobotDirection);

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

        _maze.set(rotateCell(currentCell, _currentRobotDirection), _currentRobotPos);

        if (_currentRobotPos.x == CELL_END_X && _currentRobotPos.y == CELL_END_Y)
        {
            runCyclogram(Rotate180OnPlace);

            _solver.findPath(Vec2Int(0, 0), _currentRobotPos, &_maze);

            isRunToBase = true;
        }
    }
    else
    {
        MazeSolver::CellState state = _solver.getCellState(_currentRobotPos);

        if (state == MazeSolver::END)
            return;

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
}