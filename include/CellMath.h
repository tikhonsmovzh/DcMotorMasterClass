#pragma once

#include "MazeSolver.h"
#include "Maze.h"

enum SimpleAction
{
    MOVE_FORWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_REVERS,
    STOP
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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

void calcForwardPos(Direction *dir, Vec2Int *pos)
{
    switch (*dir)
    {
    case UP:
        pos->y--;
        break;

    case DOWN:
        pos->y++;
        break;

    case LEFT:
        pos->x--;
        break;

    case RIGHT:
        pos->x++;
        break;
    }
}

void calcLeftPos(Direction *dir, Vec2Int *pos)
{
    switch (*dir)
    {
    case UP:
        pos->x--;
        *dir = LEFT;
        break;

    case DOWN:
        pos->x++;
        *dir = RIGHT;
        break;

    case LEFT:
        pos->y++;
        *dir = DOWN;
        break;

    case RIGHT:
        pos->y--;
        *dir = UP;
        break;
    }
}

void calcRightPos(Direction *dir, Vec2Int *pos)
{
    switch (*dir)
    {
    case UP:
        pos->x++;
        *dir = RIGHT;
        break;

    case DOWN:
        pos->x--;
        *dir = LEFT;
        break;

    case LEFT:
        pos->y--;
        *dir = UP;
        break;

    case RIGHT:
        pos->y++;
        *dir = DOWN;
        break;
    }
}

void calcReversPos(Direction *dir, Vec2Int *pos)
{
    switch (*dir)
    {
    case UP:
        pos->y++;
        *dir = DOWN;
        break;

    case DOWN:
        pos->y--;
        *dir = UP;
        break;

    case LEFT:
        pos->x++;
        *dir = RIGHT;
        break;

    case RIGHT:
        pos->x--;
        *dir = LEFT;
        break;
    }
}

SimpleAction converCellStateToAction(MazeSolver::CellState cellState, Direction direction)
{
    switch (direction)
    {
    case UP:
        if (cellState == MazeSolver::UP)
            return MOVE_FORWARD;
        else if (cellState == MazeSolver::LEFT)
            return MOVE_LEFT;
        else if (cellState == MazeSolver::RIGHT)
            return MOVE_RIGHT;
        else
            return MOVE_REVERS;

    case LEFT:
        if (cellState == MazeSolver::UP)
            return MOVE_RIGHT;
        else if (cellState == MazeSolver::LEFT)
            return MOVE_FORWARD;
        else if (cellState == MazeSolver::DOWN)
            return MOVE_LEFT;
        else
            return MOVE_REVERS;

        break;

    case RIGHT:
        if (cellState == MazeSolver::UP)
            return MOVE_LEFT;
        else if (cellState == MazeSolver::RIGHT)
            return MOVE_FORWARD;
        else if (cellState == MazeSolver::DOWN)
            return MOVE_RIGHT;
        else
            return MOVE_REVERS;

        break;

    case DOWN:
        if (cellState == MazeSolver::LEFT)
            return MOVE_RIGHT;
        else if (cellState == MazeSolver::DOWN)
            return MOVE_FORWARD;
        else if (cellState == MazeSolver::RIGHT)
            return MOVE_LEFT;
        else
            return MOVE_REVERS;

        break;
    }

    return MOVE_FORWARD;
}