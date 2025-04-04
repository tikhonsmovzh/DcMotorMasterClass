#pragma once

#include "Maze.h"
#include "MazeSolver.h"
#include "Config.h"
#include "Arduino.h"
#include "Vec2Int.h"

void drawMaze(Maze *maze, MazeSolver *solver)
{
    Serial.print(CORNER);

    for (int x = 0; x < MAZE_SIZE_X; x++)
    {
        Serial.print(HORIZONTAL_WALL);
        Serial.print(CORNER);
    }

    Serial.println("");

    for (int y = 0; y < MAZE_SIZE_Y * 2; y++)
    {
        if (y % 2 == 1)
            Serial.print(CORNER);
        else
            Serial.print(VERITICAL_WALL);

        for (int x = 0; x < MAZE_SIZE_X; x++)
        {
            Vec2Int curPos = Vec2Int(x, y / 2);

            Maze::Cell cell = maze->get(curPos);

            if (y % 2 == 0)
            {
                if (solver != nullptr)
                {
                    char state;

                    switch (solver->getCellState(curPos))
                    {
                    case MazeSolver::UKNNOWN:
                        state = 'X';
                        break;

                    case MazeSolver::RIGHT:
                        state = 'R';
                        break;

                    case MazeSolver::LEFT:
                        state = 'L';
                        break;

                    case MazeSolver::DOWN:
                        state = 'D';
                        break;

                    case MazeSolver::UP:
                        state = 'U';
                        break;

                    case MazeSolver::END:
                        state = 'E';
                        break;
                    }

                    Serial.print(" ");
                    Serial.print(state);
                    Serial.print(" ");
                }
                else
                    Serial.print("   ");

                if (cell.right == Maze::WALL)
                    Serial.print(VERITICAL_WALL);
                else if (cell.right == Maze::UNKNOWN)
                    Serial.print(VERITICAL_WALL_UNKNOWN);
                else
                    Serial.print(VERITICAL_WALL_EMPTY);
            }
            else
            {
                if (cell.down == Maze::WALL)
                    Serial.print(HORIZONTAL_WALL);
                else if (cell.down == Maze::UNKNOWN)
                    Serial.print(HORIZONTAL_WALL_UNKNOWN);
                else
                    Serial.print(HORIZONTAL_WALL_ENPTY);

                Serial.print(CORNER);
            }
        }

        Serial.println("");
    }
}