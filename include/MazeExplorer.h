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

enum FastState
{
    ORTHO_L,
    ORTHO_R,
    ORTHO_LL,
    ORTHO_RR,
    ORTHO,
    DIAG_RL,
    DIAG_LR,
    DIAG_RR,
    DIAG_LL
};

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

    int x = 0;

    addCyclogramToQueue(&START_CENTER);

    FastState currentFastState = ORTHO;

    optimalTrajectory.remove(0);

    while (optimalTrajectory.size() > 0)
    {
        SimpleAction action = optimalTrajectory[0];
        optimalTrajectory.remove(0);

        // Serial.println("");

        // if (action == MOVE_FORWARD)
        //     Serial.println("forward");
        // else if (action == MOVE_LEFT)
        //     Serial.println("left");
        // else if (action == MOVE_RIGHT)
        //     Serial.println("right");

        // Serial.println();

        switch (currentFastState)
        {
        case ORTHO:
        {
            if (action == MOVE_FORWARD)
                x++;
            else if (action == MOVE_RIGHT)
            {
                for (int i = 0; i < x; i++)
                {
                    addCyclogramToQueue(&FAST_FORWARD);

                    Serial.println("Forward");
                }

                currentFastState = ORTHO_R;
            }
            else if (action == MOVE_LEFT)
            {
                for (int i = 0; i < x; i++)
                {
                    addCyclogramToQueue(&FAST_FORWARD);

                    Serial.println("Forward");
                }

                currentFastState = ORTHO_L;
            }
            else if (action == STOP)
                for (int i = 0; i < x; i++)
                {
                    addCyclogramToQueue(&FAST_FORWARD);

                    Serial.println("Forward");
                }

            break;
        }

        case ORTHO_R:
        {
            if (action == MOVE_FORWARD)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 90 Right");

                x = 0;

                currentFastState = ORTHO;
            }
            else if (action == MOVE_RIGHT)
                currentFastState = ORTHO_RR;
            else if (action == MOVE_LEFT)
            {
                addCyclogramToQueue(&ROTATE_45_RIGHT);

                Serial.println("rotate 45 Right");

                x = 0;

                currentFastState = DIAG_RL;
            }
            else if (action == STOP)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 90 Right");

                addCyclogramToQueue(&FAST_FORWARD);

                Serial.println("Forward");
            }

            break;
        }

        case ORTHO_L:
        {
            if (action == MOVE_FORWARD)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 90 Left");

                x = 0;

                currentFastState = ORTHO;
            }
            else if (action == MOVE_LEFT)
                currentFastState = ORTHO_LL;
            else if (action == MOVE_RIGHT)
            {
                addCyclogramToQueue(&ROTATE_45_LEFT);

                Serial.println("Rotate 45 left");
                x = 0;

                currentFastState = DIAG_LR;
            }
            else if (action == STOP)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 90 Left");

                addCyclogramToQueue(&FAST_FORWARD);
                Serial.println("forward");
            }

            break;
        }

        case ORTHO_RR:
        {
            if (action == MOVE_FORWARD)
            {
                x = 0;
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 180 right");

                currentFastState = ORTHO;
            }
            else if (action == MOVE_LEFT)
            {
                x = 0;
                addCyclogramToQueue(&ROTATE_135_RIGHT);

                Serial.println("rotate 135 right");

                currentFastState = DIAG_RL;
            }
            else if (action == STOP)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&ROTATE_90_RIGHT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("Rotate 180 right");

                addCyclogramToQueue(&FAST_FORWARD);

                Serial.println("Forward");
            }

            break;
        }

        case ORTHO_LL:
        {
            if (action == MOVE_FORWARD)
            {
                x = 0;
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("rotate 180 Left");

                currentFastState = ORTHO;
            }
            else if (action == MOVE_RIGHT)
            {
                x = 0;
                addCyclogramToQueue(&ROTATE_135_LEFT);

                Serial.println("rotate 135 left");

                currentFastState = DIAG_LR;
            }
            else if (action == STOP)
            {
                addCyclogramToQueue(&FAST_HALF_FORWARD);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&ROTATE_90_LEFT_FAST);
                addCyclogramToQueue(&FAST_HALF_FORWARD);

                Serial.println("rotate 180 Left");

                addCyclogramToQueue(&FAST_FORWARD);

                Serial.println("Forward");
            }

            break;
        }

        case DIAG_LR:
        {
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

                currentFastState = ORTHO;
            }
            else if (action == MOVE_RIGHT)
            {
                currentFastState = DIAG_RR;
            }
            else if (action == MOVE_LEFT)
            {
                x++;

                currentFastState = DIAG_RL;
            }
            else if (action == STOP)
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
            }

            break;
        }

        case DIAG_RL:
        {
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

                currentFastState = ORTHO;
            }
            else if (action == MOVE_LEFT)
            {
                currentFastState = DIAG_LL;
            }
            else if (action == MOVE_RIGHT)
            {
                x++;

                currentFastState = DIAG_LR;
            }
            else if (action == STOP)
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
            }

            break;
        }

        case DIAG_RR:
        {
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

                currentFastState = ORTHO;
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

                currentFastState = DIAG_RL;
            }
            else if (action == STOP)
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
            }

            break;
        }

        case DIAG_LL:
        {
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

                currentFastState = DIAG_LR;
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

                currentFastState = ORTHO;
            }
            else if (action == STOP)
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
            }

            break;
        }
        }
    }

    addCyclogramToQueue(&FAST_FORWARD);

    Serial.println("Forward");
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
            _endPoint.x = 0;
            _endPoint.y = 0;

            gCurrentRunState = SERCH_END;

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
        calcReversPos(&gCurrentRobotDirection, &gCurrentRobotPos);

        fixCounter = 0;

        addCyclogramToQueue(&ROTATE_180);
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