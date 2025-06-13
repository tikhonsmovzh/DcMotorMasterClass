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
#include "CellMath.h"

enum RunState
{
    SERCH_FINISH,
    SERCH_END,
    FAST_RUN,
    WAIT
};

Vec2Int gCurrentRobotPos(0, 0);
Direction gCurrentRobotDirection = DOWN;

RunState gCurrentRunState = WAIT;

Vec2Int _endPoint(CELL_END_X, CELL_END_Y);

void resetState()
{
    // if (gDistanceFrontRight > 250)
    // {
    gCurrentRobotPos.x = 1;
    gCurrentRobotPos.y = 0;

    gCurrentRobotDirection = RIGHT;

    gMaze.set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    // }
    // else
    // {
    //     gCurrentRobotPos.x = 0;
    //     gCurrentRobotPos.y = 1;

    //     gCurrentRobotDirection = DOWN;

    //     gMaze.set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    // }

    _endPoint.x = CELL_END_X;
    _endPoint.y = CELL_END_Y;

    addCyclogramToQueue(&START);
}

void findFastPathAndRun()
{
    static Vector<SimpleAction> optimalTrajectory;

    optimalTrajectory.clear();

    Vec2Int virtualCurrentPos;
    Direction virtualCurrentDirection = gCurrentRobotDirection;

    MazeSolver::CellState state;

    gSolver.findPath(virtualCurrentPos, Vec2Int(CELL_END_X, CELL_END_Y), &gMaze);

    do
    {
        state = gSolver.getCellState(virtualCurrentPos);

        if (state == MazeSolver::END || state == MazeSolver::UKNNOWN)
            break;

        SimpleAction action = converCellStateToAction(state, virtualCurrentDirection);

        optimalTrajectory.push_back(action);

        if (action == MOVE_FORWARD)
            calcForwardPos(&virtualCurrentDirection, &virtualCurrentPos);
        else if (action == MOVE_LEFT)
            calcLeftPos(&virtualCurrentDirection, &virtualCurrentPos);
        else if (action == MOVE_RIGHT)
            calcRightPos(&virtualCurrentDirection, &virtualCurrentPos);
        else
            calcReversPos(&virtualCurrentDirection, &virtualCurrentPos);
    } while (state != MazeSolver::END && state != MazeSolver::UKNNOWN);

    optimalTrajectory.push_back(STOP);

    addCyclogramToQueue(&START_CENTER);

    while (optimalTrajectory.size() > 0)
    {
        if (optimalTrajectory.size() >= 4 && optimalTrajectory[0] == MOVE_FORWARD && optimalTrajectory[1] == MOVE_RIGHT && optimalTrajectory[2] == MOVE_LEFT && optimalTrajectory[3] == MOVE_FORWARD)
        {
            addCyclogramToQueue(&HALF_FORWARD);
            addCyclogramToQueue(&ROTATE_45_RIGHT);
            addCyclogramToQueue(&ROTATE_45_LEFT_REVERS);
            addCyclogramToQueue(&HALF_FORWARD);
        }
        else if (optimalTrajectory[0] == MOVE_FORWARD)
            addCyclogramToQueue(&FORWARD);
        else if (optimalTrajectory[1] == MOVE_LEFT)
            addCyclogramToQueue(&ROTATE_90_LEFT);
        else
            addCyclogramToQueue(&ROTATE_90_RIGHT);
    }
}

void mazeExplorerTick()
{
    // if (gCurrentRunState == FAST_RUN)
    // {
    //     if (isCyclogramsEmpty())
    //         gCurrentRunState = WAIT;

    //     return;
    // }

    static bool isExplorored = false;

    if (gCurrentRunState == WAIT)
    {
        static bool oldIsStart = false;

        bool isStart = gDistanceFrontLeft > 260;

        if (!oldIsStart && isStart)
        {
            static int optoparStartCounter = 0;

            optoparStartCounter++;

            if (optoparStartCounter >= 2)
            {
                optoparStartCounter = 0;

                static int startsCounter = 0;

                if (isExplorored && startsCounter >= 2)
                {
                    gCurrentRunState = FAST_RUN;

                    resetState();
                }
                else
                {
                    gCurrentRunState = SERCH_FINISH;

                    resetState();
                }

                startsCounter++;
            }
        }

        oldIsStart = isStart;

        return;
    }

    if (!isCyclogramsEmpty())
        return;

    if (gCurrentRunState != FAST_RUN)
    {
        Maze::Cell currentCell = cellToLocal(gMaze.get(gCurrentRobotPos), gCurrentRobotDirection);

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

        gMaze.set(cellToGlobal(currentCell, gCurrentRobotDirection), gCurrentRobotPos);
    }

    gSolver.findPath(gCurrentRobotPos, _endPoint, &gMaze);

    MazeSolver::CellState state = gSolver.getCellState(gCurrentRobotPos);

    Serial.println(state);

    if (state == MazeSolver::END || state == MazeSolver::UKNNOWN)
    {
        if (gCurrentRunState == SERCH_FINISH || gCurrentRunState == FAST_RUN)
        {
            if (gCurrentFunction != 8)
            {
                _endPoint.x = 0;
                _endPoint.y = 0;

                gCurrentRunState = SERCH_END;
            }
            else
                gCurrentRunState = WAIT;

            isExplorored = true;
        }
        else if (gCurrentRunState == SERCH_END)
            gCurrentRunState = WAIT;

        return;
    }

    SimpleAction action = converCellStateToAction(state, gCurrentRobotDirection);

    static int fixCounter = 0;

    Serial.println(action);

    if (action == MOVE_REVERS)
    {
        Maze::Cell currentCell = cellToLocal(gMaze.get(gCurrentRobotPos), gCurrentRobotDirection);

        calcReversPos(&gCurrentRobotDirection, &gCurrentRobotPos);

        fixCounter = 0;

        if (currentCell.up == Maze::WALL)
            addCyclogramToQueue(&ROTATE_180);
        else
            addCyclogramToQueue(&ROTATE_180_ON_PLACE);
    }
    else if (action == MOVE_RIGHT)
    {
        calcRightPos(&gCurrentRobotDirection, &gCurrentRobotPos);

        fixCounter++;

        if (fixCounter % 2 == 0)
        {
            if (gCurrentRunState == FAST_RUN)
                addCyclogramToQueue(&ROTATE_90_RIGHT_FIX_FAST);
            else
                addCyclogramToQueue(&ROTATE_90_RIGHT_FIX);
        }
        else
        {
            if (gCurrentRunState == FAST_RUN)
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
            else
                addCyclogramToQueue(&ROTATE_90_RIGHT);
        }
    }
    else if (action == MOVE_LEFT)
    {
        calcLeftPos(&gCurrentRobotDirection, &gCurrentRobotPos);

        fixCounter++;

        if (fixCounter % 2 == 0)
        {
            if (gCurrentRunState == FAST_RUN)
                addCyclogramToQueue(&ROTATE_90_LEFT_FIX_FAST);
            else
                addCyclogramToQueue(&ROTATE_90_LEFT_FIX);
        }
        else
        {
            if (gCurrentRunState == FAST_RUN)
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
            else
                addCyclogramToQueue(&ROTATE_90_LEFT);
        }
    }
    else
    {
        calcForwardPos(&gCurrentRobotDirection, &gCurrentRobotPos);

        fixCounter = 0;

        if (gCurrentRunState == FAST_RUN)
            addCyclogramToQueue(&FAST_FORWARD);
        else
            addCyclogramToQueue(&FORWARD);
    }

    if (fixCounter % 4 == 0 && (action == MOVE_LEFT || action == MOVE_RIGHT))
    {
        Maze::Cell currentCell = cellToLocal(gMaze.get(gCurrentRobotPos), gCurrentRobotDirection);

        if (currentCell.right == Maze::WALL)
            addCyclogramToQueue(&START);
    }
}