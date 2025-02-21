#pragma once

struct Vec2Int
{
    int x, y;

    Vec2Int(int x, int y){
        this->x = x;
        this->y = y;
    }

    Vec2Int(){
        x = 0;
        y = 0;
    }
};
