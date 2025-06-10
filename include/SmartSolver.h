#pragma once

#include <Arduino.h>
#include "Config.h"
#include "MazeExplorer.h"
#include "Maze.h"
#include "MazeDrawer.h"
#include "MazeSolver.h"
#include "FunctionSwitcher.h"
#include "Vec2Int.h"

Vec2Int _virtualCurrentPos;
Direction _virtualCurrentDirection;

enum SimpleAction
{
    MOVE_FORWARD,
    MOVE_LEFT,
    MOVE_RIGHT
};

Vector<SimpleAction> _optimalTrajectory;

int x = 1;

void virtualMoveToForward()
{
    _optimalTrajectory.push_back(MOVE_FORWARD);

    switch (_virtualCurrentDirection)
    {
    case UP:
        _virtualCurrentPos.y--;
        break;

    case DOWN:
        _virtualCurrentPos.y++;
        break;

    case LEFT:
        _virtualCurrentPos.x--;
        break;

    case RIGHT:
        _virtualCurrentPos.x++;
        break;
    }
}

void virtualMoveToLeft()
{
    _optimalTrajectory.push_back(MOVE_LEFT);

    switch (_virtualCurrentDirection)
    {
    case UP:
        _virtualCurrentPos.x--;
        _virtualCurrentDirection = LEFT;
        break;

    case DOWN:
        _virtualCurrentPos.x++;
        _virtualCurrentDirection = RIGHT;
        break;

    case LEFT:
        _virtualCurrentPos.y++;
        _virtualCurrentDirection = DOWN;
        break;

    case RIGHT:
        _virtualCurrentPos.y--;
        _virtualCurrentDirection = UP;
        break;
    }
}

void virtualMoveToRight()
{
    _optimalTrajectory.push_back(MOVE_RIGHT);

    switch (_virtualCurrentDirection)
    {
    case UP:
        _virtualCurrentPos.x++;
        _virtualCurrentDirection = RIGHT;
        break;

    case DOWN:
        _virtualCurrentPos.x--;
        _virtualCurrentDirection = LEFT;
        break;

    case LEFT:
        _virtualCurrentPos.y--;
        _virtualCurrentDirection = UP;
        break;

    case RIGHT:
        _virtualCurrentPos.y++;
        _virtualCurrentDirection = DOWN;
        break;
    }
}

void ORTHO_L();
void ORTHO_R();
void ORTHO_LL();
void ORTHO_RR();
void ORTHO();
void DIAG_RL();
void DIAG_LR();
void DIAG_RR();
void DIAG_LL();

void findFastPath()
{
    _virtualCurrentPos.x = 0;
    _virtualCurrentPos.y = 0;

    if (gCurrentFunction == 8)
        _virtualCurrentDirection = RIGHT;
    else
        _virtualCurrentDirection = DOWN;

    gSolver.findPath(_virtualCurrentPos, Vec2Int(CELL_END_X, CELL_END_Y), &gMaze);

    _optimalTrajectory.clear();

    MazeSolver::CellState state;

    do
    {
        state = gSolver.getCellState(_virtualCurrentPos);

        switch (_virtualCurrentDirection)
        {
        case UP:
            if (state == MazeSolver::UP)
                virtualMoveToForward();
            else if (state == MazeSolver::LEFT)
                virtualMoveToLeft();
            else if (state == MazeSolver::RIGHT)
                virtualMoveToRight();

            break;

        case LEFT:
            if (state == MazeSolver::UP)
                virtualMoveToRight();
            else if (state == MazeSolver::LEFT)
                virtualMoveToForward();
            else if (state == MazeSolver::DOWN)
                virtualMoveToLeft();

            break;

        case RIGHT:
            if (state == MazeSolver::UP)
                virtualMoveToLeft();
            else if (state == MazeSolver::RIGHT)
                virtualMoveToForward();
            else if (state == MazeSolver::DOWN)
                virtualMoveToRight();

            break;

        case DOWN:
            if (state == MazeSolver::LEFT)
                virtualMoveToRight();
            else if (state == MazeSolver::DOWN)
                virtualMoveToForward();
            else if (state == MazeSolver::RIGHT)
                virtualMoveToLeft();

            break;
        }
    } while (state != MazeSolver::END && state != MazeSolver::UKNNOWN);

    addCyclogramToQueue(&START_CENTER);

    x = 0;

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
        ORTHO();
}

void ORTHO()
{
    if (_optimalTrajectory.size() == 0)
        return;

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        x++;
        ORTHO();
    }
    else if (action == MOVE_RIGHT)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FAST_FORWARD);

            Serial.println("Forward");
        }

        ORTHO_R();
    }
    else if (action == MOVE_LEFT)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FAST_FORWARD);

            Serial.println("Forward");
        }

        ORTHO_L();
    }
}

void ORTHO_R()
{
    if (_optimalTrajectory.size() == 0)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 90 Right");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 90 Right");

        x = 0;

        ORTHO();
    }
    else if (action == MOVE_RIGHT)
    {
        ORTHO_RR();
    }
    else if (action == MOVE_LEFT)
    {
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);

        Serial.println("rotate 45 Right");

        x = 0;

        DIAG_RL();
    }
}

void ORTHO_L()
{
    if (_optimalTrajectory.size() == 0)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 90 Left");

        addCyclogramToQueue(&FAST_FORWARD);
        Serial.println("forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 90 Left");

        x = 0;

        ORTHO();
    }
    else if (action == MOVE_LEFT)
    {
        ORTHO_LL();
    }
    else if (action == MOVE_RIGHT)
    {
        addCyclogramToQueue(&ROTATE_45_LEFT);

        Serial.println("Rotate 45 left");
        x = 0;
        DIAG_LR();
    }
}

void ORTHO_RR()
{
    if (_optimalTrajectory.size() == 0)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 180 right");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        x = 0;
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("Rotate 180 right");

        ORTHO();
    }
    else if (action == MOVE_LEFT)
    {
        x = 0;
        addCyclogramToQueue(&ROTATE_135_RIGHT);

        Serial.println("rotate 135 right");

        DIAG_RL();
    }
}

void ORTHO_LL()
{
    if (_optimalTrajectory.size() == 0)
    {
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("rotate 180 Left");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        x = 0;
        addCyclogramToQueue(&FAST_HALF_FORWARD);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
        addCyclogramToQueue(&FAST_HALF_FORWARD);

        Serial.println("rotate 180 Left");

        ORTHO();
    }
    else if (action == MOVE_RIGHT)
    {
        x = 0;
        addCyclogramToQueue(&ROTATE_135_LEFT);

        Serial.println("rotate 135 left");

        DIAG_LR();
    }
}

void DIAG_LR()
{
    if (_optimalTrajectory.size() == 0)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);
            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_45_RIGHT_REVERS);

        Serial.println("Rotate 45 right revers");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);
            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_45_RIGHT_REVERS);

        Serial.println("Rotate 45 Right revers");

        x = 0;

        ORTHO();
    }
    else if (action == MOVE_RIGHT)
    {
        DIAG_RR();
    }
    else if (action == MOVE_LEFT)
    {
        x++;

        DIAG_RL();
    }
}

void DIAG_RL()
{
    if (_optimalTrajectory.size() == 0)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_45_LEFT_REVERS);

        Serial.println("rotate 45 left revers");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_45_LEFT_REVERS);

        Serial.println("rotate 45 left revers");

        x = 0;

        ORTHO();
    }
    else if (action == MOVE_LEFT)
    {
        DIAG_LL();
    }
    else if (action == MOVE_RIGHT)
    {
        x++;

        DIAG_LR();
    }
}

void DIAG_RR()
{
    if (_optimalTrajectory.size() == 0)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);
            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_135_RIGHT_REVERS);

        Serial.println("rotate 135 right revers");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("forward 45");
        }

        addCyclogramToQueue(&ROTATE_135_RIGHT_REVERS);

        Serial.println("rotate 135 right revers");

        x = 0;

        ORTHO();
    }
    else if (action == MOVE_LEFT)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&DIAGONAL_90_RIGHT);

        Serial.println("diagonal 90 right");

        x = 0;

        DIAG_RL();
    }
}

void DIAG_LL()
{
    if (_optimalTrajectory.size() == 0)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("forward 45");
        }

        addCyclogramToQueue(&ROTATE_135_LEFT_REVERS);

        Serial.println("rotate 135 left revers");

        addCyclogramToQueue(&FAST_FORWARD);

        Serial.println("Forward");

        return;
    }

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_RIGHT)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&DIAGONAL_90_LEFT);

        Serial.println("dagonal 90 left");

        x = 0;

        DIAG_LR();
    }
    else if (action == MOVE_FORWARD)
    {
        for (int i = 0; i < x; i++)
        {
            addCyclogramToQueue(&FORWARD_45_FAST);

            Serial.println("Forward 45");
        }

        addCyclogramToQueue(&ROTATE_135_LEFT_REVERS);
        Serial.println("Rotate 135 left revers");

        x = 0;

        ORTHO();
    }
}

void smartSolverTick()
{
    if (gCurrentRunState == FAST_RUN)
    {
        if (isCyclogramsEmpty())
        {
            _endPoint.x = 0;
            _endPoint.y = 0;
            _currentRobotDirection = _virtualCurrentDirection;
            _currentRobotPos = _virtualCurrentPos;
            gCurrentRunState = SERCH_END;
        }
    }
}