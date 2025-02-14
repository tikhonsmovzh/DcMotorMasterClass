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

    Cell get(int x, int y) const
    {
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

    void display() const
    {
        Serial.print("+");

        for(int x = 0; x < MAZE_SIZE_X; x++){
            Serial.print("----+");
        }

        Serial.println("");

        for(int y = 0; y < MAZE_SIZE_Y * 2; y++){
            if(y % 2 == 1)
                Serial.print("+");
            else
                Serial.print("|");

            for(int x = 0; x < MAZE_SIZE_X; x++){
                Cell cell = get(x, y / 2);

                if(y % 2 == 0){
                    Serial.print("    ");

                    if(cell.right == WALL)
                        Serial.print("|");
                    else if(cell.right == UNKNOWN)
                        Serial.print(".");
                    else
                        Serial.print(" ");
                }
                else{
                    if(cell.down == WALL)
                        Serial.print("----");
                    else if(cell.down == UNKNOWN)
                        Serial.print(" .. ");
                    else
                        Serial.print("    ");

                    Serial.print("+");
                }
            }

            Serial.println("");
        }
    }
};