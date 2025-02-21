#pragma once

#include "Vec2Int.h"
#include "Maze.h"
#include <Arduino.h>

void maze1(Maze *maze){
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(0, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(1, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(2, 0));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 0));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 0));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(5, 0));
    
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 1));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(1, 1));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::EMPTY), Vec2Int(2, 1));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(4, 1));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(5, 1));
  
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(0, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(1, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(2, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 2));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(4, 2));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(5, 2));
  
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(0, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(1, 3));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(2, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 3));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::EMPTY), Vec2Int(4, 3));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(5, 3));
  
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::WALL), Vec2Int(0, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(1, 4));
    maze->set(Maze::Cell(Maze::EMPTY, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(2, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::WALL), Vec2Int(3, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 4));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::EMPTY, Maze::EMPTY), Vec2Int(5, 4));
    
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(0, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(1, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::EMPTY, Maze::WALL), Vec2Int(2, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(3, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::EMPTY, Maze::WALL, Maze::EMPTY), Vec2Int(4, 5));
    maze->set(Maze::Cell(Maze::WALL, Maze::WALL, Maze::WALL, Maze::EMPTY), Vec2Int(5, 5));
}

void generateMaze(int width, int height, Maze *maze) {
    // Инициализация всех внутренних стен как WALL
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Maze::Cell cell = maze->get(Vec2Int(x, y));
            if (x < width - 1 && cell.right == Maze::UNKNOWN) 
                cell.right = Maze::WALL;
            if (y < height - 1 && cell.down == Maze::UNKNOWN)
                cell.down = Maze::WALL;
            maze->set(cell, Vec2Int(x, y));
        }
    }

    // Инициализация структур данных
    bool** visited = new bool*[height];
    for (int y = 0; y < height; y++) {
        visited[y] = new bool[width]{false};
    }

    // Направления: вверх, вправо, вниз, влево
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    // Стек для DFS
    struct StackNode { int x, y; };
    StackNode* stack = new StackNode[width * height];
    int stackSize = 0;
    stack[stackSize++] = {0, 0};
    visited[0][0] = true;

    while (stackSize > 0) {
        int x = stack[stackSize-1].x;
        int y = stack[stackSize-1].y;
        
        // Поиск доступных направлений
        int available[4];
        int count = 0;
        
        for (int dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[ny][nx]) {
                available[count++] = dir;
            }
        }

        if (count > 0) {
            // Выбор случайного направления
            int dir = available[random(0, count-1)];
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // Удаляем стену между клетками
            Maze::Cell curr_cell = maze->get(Vec2Int(x, y));
            switch(dir) {
                case 0: // Вверх
                    curr_cell.up = Maze::EMPTY;
                    break;
                case 1: // Вправо
                    curr_cell.right = Maze::EMPTY;
                    break;
                case 2: // Вниз
                    curr_cell.down = Maze::EMPTY;
                    break;
                case 3: // Влево
                    curr_cell.left = Maze::EMPTY;
                    break;
            }
            maze->set(curr_cell, Vec2Int(x, y));

            // Переходим в новую клетку
            visited[ny][nx] = true;
            stack[stackSize++] = {nx, ny};
        } else {
            // Откат
            stackSize--;
        }
    }

    // Освобождение памяти
    for (int y = 0; y < height; y++) {
        delete[] visited[y];
    }
    delete[] visited;
    delete[] stack;
}