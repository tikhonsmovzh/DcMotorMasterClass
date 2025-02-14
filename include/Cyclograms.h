#pragma once

#include <Arduino.h>
#include "Config.h"
#include "DriveTrain.h"

struct Sensor{
    float time = 0.0;
};

struct MotorState
{
    float forwardVel = 0.0f;
    float headingVelocity = 0.0f;

    bool isComplited = false;
};


class ICyclogram{
public:
    virtual void run(Sensor, MotorState*) = 0;
};

class Forward: public ICyclogram{
    void run(Sensor sensor, MotorState *motorState){
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;

        if(sensor.time > CELL_SIZE / FORWARD_VEL)
            motorState->isComplited = true;
    }
};

class Idle: public ICyclogram{
    void run(Sensor sensor, MotorState *motorState){
        motorState->forwardVel = 0.0;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
};

class Stop: public ICyclogram{
    void run(Sensor sensor, MotorState *motorState){
        motorState->forwardVel = 0.0;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = true;
    }
};

class RotateSS90: public ICyclogram{
    void run(Sensor sensor, MotorState *motorState){
        const float radius = (CELL_SIZE * 0.5f);

        const float headingVel = FORWARD_VEL / radius;
        motorState->forwardVel = FORWARD_VEL;

        const float forwardTime = (CELL_SIZE * 0.5f) / FORWARD_VEL;
        const float rotTime = (2.0f * PI * radius * 0.25f) / FORWARD_VEL;

        if(sensor.time < forwardTime || sensor.time > forwardTime + rotTime)
            motorState->headingVelocity = 0.0;
        else
            motorState->headingVelocity = headingVel;

        if(rotTime + forwardTime * 2.0f < sensor.time)
            motorState->isComplited = true;
    }
};

ICyclogram* _cyclograms[BUFFER_LENGHT] = {/*new RotateSS90(),*/ new Idle()};
int _currentCyclogram = 0;
float _lastCyclogramTime = 0.0;

void cyclogramsInit(){
    _lastCyclogramTime = millis() / 1000.0f;
}

void cyclogramsTick(){
    MotorState motorState = MotorState();
    Sensor sensor = Sensor();
    
    sensor.time = millis() / 1000.0f - _lastCyclogramTime;

    _cyclograms[_currentCyclogram]->run(sensor, &motorState);

    setDriveVelocity(motorState.forwardVel, motorState.headingVelocity);

    if(motorState.isComplited){
        _currentCyclogram++;
        _currentCyclogram %= BUFFER_LENGHT;
        
        _lastCyclogramTime = millis() / 1000.0f;
    }
}