#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Maze.h"
#include "Queue.h"
#include "Vec2Int.h"

class MazeSolver
{
public:
    enum CellState
    {
        UKNNOWN,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        END
    };

private:
    CellState cellStates[MAZE_SIZE_X][MAZE_SIZE_Y] = {UKNNOWN};

public:
    void findPath(Vec2Int start, Vec2Int end, Maze *maze)
    {
        Queue<Vec2Int> queue;

        for (int x = 0; x < MAZE_SIZE_X; x++)
            for (int y = 0; y < MAZE_SIZE_Y; y++)
                cellStates[x][y] = UKNNOWN;

        queue.pushBack(end);

        cellStates[end.x][end.y] = END;

        while (!queue.isEmpty())
        {
            Vec2Int current = queue.popFront();

            Maze::Cell cell = maze->get(current);

            if (cell.left != Maze::WALL)
            {
                Vec2Int left = Vec2Int(current.x - 1, current.y);

                if (cellStates[left.x][left.y] == UKNNOWN)
                {
                    cellStates[left.x][left.y] = RIGHT;

                    if (left.x == start.x && left.y == start.y)
                        return;

                    queue.pushBack(left);
                }
            }
            
            if (cell.up != Maze::WALL)
            {
                Vec2Int up = Vec2Int(current.x, current.y - 1);

                if (cellStates[up.x][up.y] == UKNNOWN)
                {
                    cellStates[up.x][up.y] = DOWN;

                    if (up.x == start.x && up.y == start.y)
                        return;

                    queue.pushBack(up);
                }
            }

            if (cell.right != Maze::WALL)
            {
                Vec2Int right = Vec2Int(current.x + 1, current.y);

                if (cellStates[right.x][right.y] == UKNNOWN)
                {
                    cellStates[right.x][right.y] = LEFT;

                    if (right.x == start.x && right.y == start.y)
                        return;

                    queue.pushBack(right);
                }
            }

            if (cell.down != Maze::WALL)
            {
                Vec2Int down = Vec2Int(current.x, current.y + 1);

                if (cellStates[down.x][down.y] == UKNNOWN)
                {
                    cellStates[down.x][down.y] = UP;

                    if (down.x == start.x && down.y == start.y)
                        return;

                    queue.pushBack(down);
                }
            }
        }
    }

    CellState getCellState(Vec2Int pos)
    {
        return cellStates[pos.x][pos.y];
    }
};

MazeSolver gSolver = MazeSolver();