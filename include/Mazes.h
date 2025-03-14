#pragma once

#include "Vec2Int.h"
#include "Maze.h"
#include "Queue.h"
#include "Vector.h"
#include <Arduino.h>

void maze1(Maze *maze)
{
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(2, 0));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 0));

    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(1, 1));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(5, 1));

    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(0, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(2, 2));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(4, 2));
    
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(1, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 3));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(5, 3));

    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 4));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(2, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 4));
  
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(1, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(5, 5));
}

void maze2(Maze *maze)
{
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(2, 0));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(6, 0));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(8, 0));
    
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(1, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(3, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(5, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(7, 1));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(9, 1));

    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 2));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(2, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(6, 2));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(8, 2));
    
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(1, 3));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(5, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(7, 3));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(9, 3));
    
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(0, 4));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(2, 4));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(4, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(6, 4));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(8, 4));

    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(1, 5));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(3, 5));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(5, 5));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(7, 5));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(9, 5));
    
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 6));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(2, 6));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(4, 6));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(6, 6));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(8, 6));
    
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(1, 7));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 7));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(5, 7));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(7, 7));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(9, 7));

    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 8));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(2, 8));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::EMPTY), Vec2Int(4, 8));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(6, 8));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(8, 8));
    
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(1, 9));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(3, 9));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(5, 9));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::EMPTY), Vec2Int(7, 9));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(9, 9));
}