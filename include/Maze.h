#pragma once

#include "Config.h"
#include "Vec2Int.h"

class Maze{  
public:
    enum WallType{
        WALL,
        EMPTY,
        UNKNOWN
    };

private:
    struct RawCell
    {
        WallType down: 2;
        WallType right: 2;
    };

    RawCell _maze[MAZE_SIZE_X][MAZE_SIZE_Y];

public:
    struct Cell
    {
        WallType up: 2;
        WallType left: 2;
        WallType right: 2;
        WallType down: 2;

        Cell(WallType down, WallType right, WallType up, WallType left){
            this->up = up;
            this->down = down;
            this->left = left;
            this->right = right;
        }

        Cell(){
            up = UNKNOWN;
            down = UNKNOWN;
            left = UNKNOWN;
            right = UNKNOWN;
        }
    };

    Maze(){
        for(int x = 0; x < MAZE_SIZE_X; x++){
            for(int y = 0; y < MAZE_SIZE_Y; y++){
                _maze[x][y] = RawCell();

                _maze[x][y].down = UNKNOWN;
                _maze[x][y].right = UNKNOWN;
            }
        }

        for(int x = 0; x < MAZE_SIZE_X; x++)
            _maze[x][MAZE_SIZE_Y - 1].down = WALL;

        for(int y = 0; y < MAZE_SIZE_Y; y++)
            _maze[MAZE_SIZE_X - 1][y].right = WALL;
    }

    Cell get(Vec2Int pos) const
    {
        WallType up;
        WallType left;

        if(pos.x == 0)
            left = WALL;
        else
            left = _maze[pos.x - 1][pos.y].right;

        if(pos.y == 0)
            up = WALL;
        else
            up = _maze[pos.x][pos.y - 1].down;

        return Cell(_maze[pos.x][pos.y].down, _maze[pos.x][pos.y].right, up, left);
    }

    void set(Cell cell, Vec2Int pos){
        if(pos.y > 0)
            _maze[pos.x][pos.y - 1].down = cell.up;
        
        if(pos.x > 0)
            _maze[pos.x - 1][pos.y].right = cell.left;

        if(pos.x < MAZE_SIZE_X - 1)
            _maze[pos.x][pos.y].right = cell.right;

        if(pos.y < MAZE_SIZE_Y - 1)
            _maze[pos.x][pos.y].down = cell.down;
    }
};

Maze gMaze = Maze();