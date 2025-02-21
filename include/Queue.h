#pragma once

#include "Config.h"

template <typename T>
class Queue
{
private:
    T arr[MAX_QUEUE_SIZE];
    int begin;
    int end;

public:
    Queue()
    {
        begin = 0;
        end = 0;
    }

    bool isEmpty()
    {
        return begin == end;
    }

    bool isFull()
    {
        return isEmpty();
    }

    void pushBack(T x)
    {
        arr[end] = x;
        end = (end + 1) % MAX_QUEUE_SIZE;
    }

    T popFront()
    {
        T x = arr[begin];
        begin = (begin + 1) % MAX_QUEUE_SIZE;
        return x;
    }

    void clear()
    {
        begin = 0;
        end = 0;
    }
};