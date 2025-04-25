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

void callibrateRot(Sensor sensor, MotorState *motorState)
{
    bool leftWall = gDistanceDiagonalLeft > DIAGONAL_WALL_TRIGGER_LEFT_DISTANCE;
    bool rightWall = gDistanceDiagonalRight > DIAGONAL_WALL_TRIGGER_RIGHT_DISTANCE;

    if(!leftWall && !rightWall){
        motorState->headingVelocity = 0.0;
        return;
    }

    int16_t err = TARGET_FORWARD_DISTANCE_LEFT - sensor.distanceDiagonalLeft;

    if ((sensor.distanceDiagonalLeft < sensor.distanceDiagonalRight || !leftWall) && rightWall)
        err = -(TARGET_FORWARD_DISTANCE_RIGHT - sensor.distanceDiagonalRight);

    float u = err * FORWARD_CYCLOGRAM_P;

    motorState->headingVelocity = u;
}

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

        callibrateRot(sensor, motorState);

        if (sensor.time > CELL_SIZE / FORWARD_VEL * (_half ? 0.5f : 1.0f) * FORWARD_COLLIBREATE_K ||
            max(sensor.distanceFrontLeft, sensor.distanceFrontRight) > FORWARD_RIDE_TRIGGER_DIST)
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
        float rotTime = PI / ROTATE_VEL * ROTATE180_COLLIBREATE_K;
        float forwardTime = CELL_SIZE / FORWARD_VEL;

        float addedTime = ROTATE180_ADED_DIST / FORWARD_VEL;

        if (sensor.time < rotTime)
        {
            motorState->headingVelocity = (_direction ? 1.0 : -1.0) * ROTATE_VEL;
            motorState->forwardVel = 0.0;
        }
        else if (sensor.time < forwardTime + rotTime + addedTime)
        {
            motorState->forwardVel = -FORWARD_VEL;

            callibrateRot(sensor, motorState);
        }
        else if (sensor.time < forwardTime * 2 + addedTime + rotTime - (ROBOT_WHHEL_DIF / FORWARD_VEL))
        {
            motorState->forwardVel = FORWARD_VEL;

            callibrateRot(sensor, motorState);
        }
        else
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

class Rotate90 : public ICyclogram
{
    bool _direction;

public:
    Rotate90(bool direction = true)
    {
        _direction = direction;
    }

    void run(Sensor sensor, MotorState *motorState)
    {
        const float radius = (CELL_SIZE * 0.5f);

        const float headingVel = (_direction ? 1.0 : -1.0) * (FORWARD_VEL / radius);
        motorState->forwardVel = FORWARD_VEL;

        const float rotTime = (2.0f * PI * radius * 0.25f) / FORWARD_VEL;

        motorState->headingVelocity = headingVel;

        if (rotTime * ROTATE_COLLIBREATE_K < sensor.time)
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
        delete _cyclograms[0];
        _cyclograms.remove(0);

        setDriveVelocity(0.0, 0.0);

        _lastCyclogramTime = millis() / 1000.0f;
    }
}

bool isCyclogramsEmpty()
{
    return _cyclograms.size() == 0;
}

void addCyclogramToQueue(ICyclogram *cyclogram)
{
    _cyclograms.push_back(cyclogram);
}

void runCyclogram(ICyclogram *cyclogram)
{
    for (int i = 0; i < _cyclograms.size(); i++)
        delete _cyclograms[i];

    _cyclograms.clear();

    addCyclogramToQueue(cyclogram);
}