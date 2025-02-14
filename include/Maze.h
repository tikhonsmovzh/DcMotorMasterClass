#pragma once

#include "Config.h"
#include <Arduino.h>

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

        // RawCell(WallType down, WallType right){
        //     this->down = down;
        //     this->right = right;
        // }
    };

    RawCell _maze[MAZE_SIZE_X][MAZE_SIZE_Y];

public:
    struct Cell
    {
        WallType up;
        WallType left;
        WallType right;
        WallType down;

        Cell(WallType down, WallType right, WallType up, WallType left){
            this->up = up;
            this->down = down;
            this->left = left;
            this->right = right;
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
            _maze[x][MAZE_SIZE_Y - 1].down = UNKNOWN;

        for(int y = 0; y < MAZE_SIZE_Y; y++)
            _maze[MAZE_SIZE_X - 1][y].right = UNKNOWN;
    }

    Cell get(int x, int y){
        WallType up;
        WallType left;

        if(x == 0)
            left = WALL;
        else
            left = _maze[x - 1][y].right;

        if(y == 0)
            up = WALL;
        else
            up = _maze[x][y - 1].down;

        return Cell(_maze[x][y].down, _maze[x][y].right, up, left);
    }

    void set(Cell cell, int x, int y){
        if(y != 0)
            _maze[x][y - 1].down = cell.up;
        
        if(x != 0)
            _maze[x - 1][y].right = cell.left;

        if(x != MAZE_SIZE_X - 1)
            _maze[x][y].right = cell.right;

        if(y != MAZE_SIZE_Y - 1)
            _maze[x][y].down = cell.down;
    }

    void display(){
        for(int y = 0; y <= MAZE_SIZE_Y * 2; y++){
            for(int x = 0; x <= MAZE_SIZE_X * 4; x++){
                if(x % 4 == 0 && y % 2 == 0)
                    Serial.print("+");
                else
                {
                    if(y % 2 == 0)
                    {
                        RawCell rawCell = _maze[x / 4][y / 2];

                        if(rawCell.down == WALL)
                            Serial.print("-");
                        else
                            Serial.print(" ");
                    }
                    else
                        Serial.print(" ");
                }
            }

            Serial.println("");
        }
    }
};