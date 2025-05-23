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

Vec2Int _currentPos(0, 1);
Direction _currentRobotDirection = DOWN;

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
}

void mazeExplorerTick()
{
    if (!isCyclogramsEmpty())
        return;

    Maze::Cell currentCell = _maze.get(_currentPos);

    Maze::Cell currentLocaledCell = cellToLocal(currentCell, _currentRobotDirection);

    Maze::Cell savedLocatedCell = currentLocaledCell;

    if (currentLocaledCell.left == Maze::UNKNOWN)
        currentLocaledCell.left = isWallLeft() ? Maze::WALL : Maze::EMPTY;
    if (currentLocaledCell.right == Maze::UNKNOWN)
        currentLocaledCell.right = isWallRight() ? Maze::WALL : Maze::EMPTY;
    if (currentLocaledCell.up == Maze::UNKNOWN)
        currentLocaledCell.up = isWallForward() ? Maze::WALL : Maze::EMPTY;

    _maze.set(cellToLocal(currentLocaledCell, _currentRobotDirection), _currentPos);

    if (savedLocatedCell.left == Maze::UNKNOWN && currentLocaledCell.left == Maze::EMPTY)
    {
        addCyclogramToQueue(new Rotate90(true));

        switch (_currentRobotDirection)
        {
        case UP:
        {
            _currentRobotDirection = LEFT;
            break;
        }

        case LEFT:
        {
            _currentRobotDirection = DOWN;
            break;
        }

        case RIGHT:
        {
            _currentRobotDirection = UP;
            break;
        }

        case DOWN:
        {
            _currentRobotDirection = RIGHT;
            break;
        }
        }
    }
    else if (savedLocatedCell.right == Maze::UNKNOWN && currentLocaledCell.right == Maze::EMPTY)
    {
        addCyclogramToQueue(new Rotate90(false));

        switch (_currentRobotDirection)
        {
        case UP:
        {
            _currentRobotDirection = RIGHT;
            break;
        }

        case LEFT:
        {
            _currentRobotDirection = UP;
            break;
        }

        case RIGHT:
        {
            _currentRobotDirection = DOWN;
            break;
        }

        case DOWN:
        {
            _currentRobotDirection = LEFT;
            break;
        }
        }
    }
    else if (savedLocatedCell.up == Maze::UNKNOWN && currentLocaledCell.up == Maze::EMPTY)
    {
        addCyclogramToQueue(new Forward(false));
    }
    else if (currentLocaledCell.left == Maze::EMPTY)
    {
        addCyclogramToQueue(new Rotate90(true));

        switch (_currentRobotDirection)
        {
        case UP:
        {
            _currentRobotDirection = LEFT;
            break;
        }

        case LEFT:
        {
            _currentRobotDirection = DOWN;
            break;
        }

        case RIGHT:
        {
            _currentRobotDirection = UP;
            break;
        }

        case DOWN:
        {
            _currentRobotDirection = RIGHT;
            break;
        }
        }
    }
    else if (currentLocaledCell.right == Maze::EMPTY)
    {
        addCyclogramToQueue(new Rotate90(false));

        switch (_currentRobotDirection)
        {
        case UP:
        {
            _currentRobotDirection = RIGHT;
            break;
        }

        case LEFT:
        {
            _currentRobotDirection = UP;
            break;
        }

        case RIGHT:
        {
            _currentRobotDirection = DOWN;
            break;
        }

        case DOWN:
        {
            _currentRobotDirection = LEFT;
            break;
        }
        }
    }
    else if (currentLocaledCell.up == Maze::EMPTY)
    {
        addCyclogramToQueue(new Forward(false));
    }
    else
    {
        addCyclogramToQueue(new Rotate180());

        switch (_currentRobotDirection)
        {
        case UP:
        {
            _currentRobotDirection = DOWN;
            break;
        }

        case LEFT:
        {
            _currentRobotDirection = RIGHT;
            break;
        }

        case RIGHT:
        {
            _currentRobotDirection = LEFT;
            break;
        }

        case DOWN:
        {
            _currentRobotDirection = UP;
            break;
        }
        }
    }

    switch (_currentRobotDirection)
    {
    case UP:
    {
        _currentPos.y--;
        break;
    }

    case LEFT:
    {
        _currentPos.x--;
        break;
    }

    case RIGHT:
    {
        _currentPos.x++;
        break;
    }

    case DOWN:
    {
        _currentPos.y++;
        break;
    }
    }

    // drawMaze(&_maze, nullptr);

    // Maze::Cell rotatedNextCell;

    // Vec2Int robotPosition(gRobotState.x / CELL_SIZE, gRobotState.y / CELL_SIZE);

    // rotatedNextCell = rotateCell(gRobotState.h, nextCell);

    // if(abs(gRobotState.h) < H_SENS)
    //     robotPosition.y++;
    // else if(abs(gRobotState.h - PI * 0.5f) < H_SENS)
    //     robotPosition.x++;
    // else if(abs(gRobotState.h - PI) < H_SENS)
    //     robotPosition.y--;
    // else
    //     robotPosition.x--;

    // Maze::Cell mazeCell = _maze.get(robotPosition);

    // if(rotatedNextCell.up == Maze::UNKNOWN)
    //     rotatedNextCell.up = mazeCell.up;

    // if(rotatedNextCell.down == Maze::UNKNOWN)
    //     rotatedNextCell.down = mazeCell.down;

    // if(rotatedNextCell.left == Maze::UNKNOWN)
    //     rotatedNextCell.left = mazeCell.left;

    // if(rotatedNextCell.right == Maze::UNKNOWN)
    //     rotatedNextCell.right = mazeCell.right;

    // _maze.set(rotatedNextCell, robotPosition);
}