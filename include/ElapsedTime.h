#pragma once

#include <Arduino.h>

class ElapseTime{
private:
    uint64_t _resetMicros = 0;
    
public:
    ElapseTime(){
        reset();
    }

    void reset(){
        _resetMicros = micros();
    }

    uint64_t timeMicros(){
        return micros() - _resetMicros;
    }

    float milliseconds(){
        return timeMicros() / 1000.0f;
    }

    float seconds(){
        return timeMicros() / 1000000.0f;
    }
};