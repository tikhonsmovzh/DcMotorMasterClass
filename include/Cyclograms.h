#pragma once

#include <Arduino.h>
#include "Config.h"
#include "DriveTrain.h"
#include "DistanceSensor.h"
#include "Vector.h"

struct Sensor
{
    float time = 0.0;

    int16_t distanceFrontLeft = 0, distanceFrontRight = 0,
            distanceDiagonalLeft = 0, distanceDiagonalRight = 0;
};

struct MotorState
{
    float forwardVel = 0.0f;
    float headingVelocity = 0.0f;

    bool isComplited = false;
};

class ICyclogram
{
public:
    virtual void run(Sensor, MotorState *) = 0;
};

class Forward : public ICyclogram
{
    bool _half = false;

public:
    Forward(bool half = false)
    {
        _half = half;
    }

    void run(Sensor sensor, MotorState *motorState)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity =
            (TARGET_FORWARD_DISTANCE - sensor.distanceDiagonalLeft) * FORWARD_CYCLOGRAM_P;

        if (sensor.time > CELL_SIZE / FORWARD_VEL * (_half ? 0.5f : 1.0f))
            motorState->isComplited = true;
    }
};

class Rotate180 : public ICyclogram
{
    bool _direction;

public:
    Rotate180(bool direction = true)
    {
        _direction = direction;
    }

    void run(Sensor sensor, MotorState *motorState)
    {
        motorState->forwardVel = 0.0;
        motorState->headingVelocity = (_direction ? 1.0 : -1.0) * ROTATE_VEL;

        if (sensor.time > PI / ROTATE_VEL)
            motorState->isComplited = true;
    }
};

class Idle : public ICyclogram
{
public:
    void run(Sensor sensor, MotorState *motorState)
    {
        motorState->forwardVel = 0.0;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
};

class Stop : public ICyclogram
{
public:
    void run(Sensor sensor, MotorState *motorState)
    {
        motorState->forwardVel = 0.0;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = true;
    }
};

class RotateSS90 : public ICyclogram
{
    bool _direction;

public:
    RotateSS90(bool direction)
    {
        _direction = direction;
    }

    void run(Sensor sensor, MotorState *motorState)
    {
        const float radius = (CELL_SIZE * 0.5f);

        const float headingVel = (_direction ? 1.0 : -1.0) * (FORWARD_VEL / radius);
        motorState->forwardVel = FORWARD_VEL;

        const float forwardTime = (CELL_SIZE * 0.5f) / FORWARD_VEL;
        const float rotTime = (2.0f * PI * radius * 0.25f) / FORWARD_VEL;

        if (sensor.time < forwardTime || sensor.time > forwardTime + rotTime)
            motorState->headingVelocity = 0.0;
        else
            motorState->headingVelocity = headingVel;

        if (rotTime + forwardTime * 2.0f < sensor.time)
            motorState->isComplited = true;
    }
};

Vector<ICyclogram *> _cyclograms;
float _lastCyclogramTime = 0.0;

void cyclogramsInit()
{
    _lastCyclogramTime = millis() / 1000.0f;
}

void cyclogramsTick()
{
    if (_cyclograms.size() == 0)
    {
        setDriveVelocity(0.0, 0.0);

        _lastCyclogramTime = millis() / 1000.0f;

        return;
    }

    MotorState motorState = MotorState();
    Sensor sensor = Sensor();

    sensor.time = millis() / 1000.0f - _lastCyclogramTime;
    sensor.distanceDiagonalLeft = gDistanceDiagonalLeft;
    sensor.distanceDiagonalRight = gDistanceDiagonalRight;
    sensor.distanceFrontLeft = gDistanceFrontLeft;
    sensor.distanceFrontRight = gDistanceFrontRight;

    _cyclograms[0]->run(sensor, &motorState);

    setDriveVelocity(motorState.forwardVel, motorState.headingVelocity);

    if (motorState.isComplited)
    {
        _cyclograms.remove(0);

        setDriveVelocity(0.0, 0.0);

        _lastCyclogramTime = millis() / 1000.0f;
    }
}

bool isCyclogramsEmpty(){
    return _cyclograms.size() == 0;
}

void addCyclogramToQueue(ICyclogram *cyclogram)
{
    _cyclograms.push_back(cyclogram);
}

void runCyclogram(ICyclogram *cyclogram)
{
    _cyclograms.clear();
    addCyclogramToQueue(cyclogram);
}