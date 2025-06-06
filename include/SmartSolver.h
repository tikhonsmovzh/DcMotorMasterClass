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

void ORTHO_F();
void ORTHO_L();
void ORTHO_R();

void findFastPath()
{
    if (gCurrentFunction == 8)
    {
        _virtualCurrentPos.x = 1;
        _virtualCurrentPos.y = 0;

        _virtualCurrentDirection = RIGHT;
    }
    else
    {
        _virtualCurrentPos.x = 0;
        _virtualCurrentPos.y = 1;

        _virtualCurrentDirection = DOWN;
    }

    gSolver.findPath(_virtualCurrentPos, Vec2Int(CELL_END_X, CELL_END_Y), &gMaze);

    _optimalTrajectory.clear();

    _optimalTrajectory.push_back(MOVE_FORWARD);

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
    } while (state == MazeSolver::END || state == MazeSolver::UKNNOWN);

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    x = 1;

    if (action == MOVE_FORWARD)
        addCyclogramToQueue(Forward);

    ORTHO_F();
}

void smartSolverTick(){
    if(gCurrentRunState == FAST_RUN){
        if(isCyclogramsEmpty()){
            _endPoint.x = 0;
            _endPoint.y = 0;
            gCurrentRunState = SERCH_END;
        }
    }
}

void ORTHO_F()
{
    if (_optimalTrajectory.size() == 0)
        return;

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        x++;
        ORTHO_F();
    }
    else if (action == MOVE_RIGHT)
    {
        for (int i = 0; i < x; i++)
            addCyclogramToQueue(Forward45);

        ORTHO_R();
    }
    else if (action == MOVE_LEFT)
    {
        for (int i = 0; i < x; i++)
            addCyclogramToQueue(Forward45);

        ORTHO_L();
    }
}

void ORTHO_R()
{
    if (_optimalTrajectory.size() == 0)
        return;

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if (action == MOVE_FORWARD)
    {
        x = 2;

        addCyclogramToQueue(Rotate90Right);

        ORTHO_F();
    }
    else if (action == MOVE_RIGHT)
    {
        addCyclogramToQueue(Rotate90Right);
        addCyclogramToQueue(Forward);

        ORTHO_R();
    }
    else if(action == MOVE_LEFT){
        addCyclogramToQueue(Rotate90Right);
        addCyclogramToQueue(Forward);

        ORTHO_L();
    }
}

void ORTHO_L()
{
    if (_optimalTrajectory.size() == 0)
        return;

    SimpleAction action = _optimalTrajectory[0];
    _optimalTrajectory.remove(0);

    if(action == MOVE_FORWARD){
        addCyclogramToQueue(Rotate90Left);

        ORTHO_F();
    }
    else if(action == MOVE_RIGHT){
        addCyclogramToQueue(Rotate90Left);
        addCyclogramToQueue(Forward);

        ORTHO_R();
    }
    else if(action == MOVE_LEFT){
        addCyclogramToQueue(Rotate90Left);
        addCyclogramToQueue(Forward);

        ORTHO_L();
    }
}