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

struct ICyclogram
{
    virtual void run(Sensor *, MotorState *) = 0;
};

void callibrateRot(Sensor *sensor, MotorState *motorState)
{
    bool leftWall = isWallLeft();

    if (!leftWall)
    {
        if (!isWallRight())
            return;

        int16_t err = -TARGET_FORWARD_DISTANCE_RIGHT + sensor->distanceDiagonalRight;

        float u = err * FORWARD_CYCLOGRAM_P;

        motorState->headingVelocity = u;

        return;
    }

    int16_t err = TARGET_FORWARD_DISTANCE_LEFT - sensor->distanceDiagonalLeft;

    float u = err * FORWARD_CYCLOGRAM_P;

    motorState->headingVelocity = u;
}

class Forward : public ICyclogram
{
private:
    bool _isFast;
    bool _isHalf;
    bool _is45;

public:
    Forward(bool fast, bool half, bool is45)
    {
        _isFast = fast;
        _isHalf = half;
        _is45 = is45;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        motorState->forwardVel = _isFast ? FAST_FORWARD_VEL : FORWARD_VEL;

        if (_is45)
            motorState->headingVelocity = 0.0f;
        else
            callibrateRot(sensor, motorState);

        if (sensor->time > (_is45 ? ((CELL_SIZE * 0.5f) * sqrtf(2.0f)) : CELL_SIZE) / FORWARD_VEL * (_isFast ? FAST_FORWARD_COLLIBREATE_K : FORWARD_COLLIBREATE_K) * (_isHalf ? FAST_FORWARD_HALF_COLLIBRATE_K : 1.0f) ||
            max(sensor->distanceFrontLeft, sensor->distanceFrontRight) > FORWARD_RIDE_TRIGGER_DIST)
            motorState->isComplited = true;
    }
};

Forward FORWARD(false, false, false);
Forward HALF_FORWARD(false, true, false);
Forward FAST_FORWARD(true, false, false);
Forward FAST_HALF_FORWARD(true, true, false);
Forward FORWARD_45(false, false, true);
Forward FORWARD_45_FAST(true, false, true);

class Rotate180 : public ICyclogram
{
public:
    void run(Sensor *sensor, MotorState *motorState) override
    {
        float rotTime = PI / ROTATE_VEL * ROTATE180_COLLIBREATE_K;
        float forwardTime = CELL_SIZE / FORWARD_VEL;

        float addedTime = ROTATE180_ADED_DIST / FORWARD_VEL;

        if (sensor->time < forwardTime / 2)
        {
            motorState->forwardVel = FORWARD_VEL;

            callibrateRot(sensor, motorState);
        }
        else if (sensor->time < rotTime + forwardTime / 2)
        {
            motorState->headingVelocity = ROTATE_VEL;
            motorState->forwardVel = 0.0;
        }
        else if (sensor->time < forwardTime + rotTime + addedTime)
        {
            motorState->forwardVel = -FORWARD_VEL;

            callibrateRot(sensor, motorState);
        }
        else if (sensor->time < forwardTime * 2 + addedTime + rotTime - (ROBOT_WHHEL_DIF / FORWARD_VEL))
        {
            motorState->forwardVel = FORWARD_VEL;

            callibrateRot(sensor, motorState);
        }
        else
            motorState->isComplited = true;
    }
};

Rotate180 ROTATE_180;

class Start : public ICyclogram
{
private:
    bool _toCenter;

public:
    Start(bool toCenter)
    {
        _toCenter = toCenter;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        float forwardTime = (CELL_SIZE - ROBOT_WHHEL_DIF) / FORWARD_VEL;
        float adedTime = ROTATE180_ADED_DIST / FORWARD_VEL;

        if (sensor->time < forwardTime + adedTime)
        {
            motorState->forwardVel = -FORWARD_VEL;
            motorState->headingVelocity = 0.0f;

            callibrateRot(sensor, motorState);
        }
        else if (sensor->time < forwardTime + adedTime + (_toCenter ? ((CELL_SIZE / 2.0f - ROBOT_WHHEL_DIF) / FORWARD_VEL) : forwardTime))
        {
            motorState->forwardVel = FORWARD_VEL;
            motorState->headingVelocity = 0.0f;

            callibrateRot(sensor, motorState);
        }
        else
            motorState->isComplited = true;
    }
};

Start START(false);
Start START_CENTER(true);

class Rotate90 : public ICyclogram
{
private:
    bool _isFast;
    bool _side; // true - left
    bool _isFixed;

public:
    Rotate90(bool fast, bool side, bool fixed)
    {
        _isFast = fast;
        _side = side;
        _isFixed = fixed;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        const float radius = (CELL_SIZE * (_isFixed ? 0.29f : 0.49f));

        const float headingVel = (_side ? 1.0f : -1.0f) * ((_isFast ? FAST_ROTATE_FORWARD_VEL : FORWARD_VEL) / radius);
        motorState->forwardVel = _isFast ? FAST_ROTATE_FORWARD_VEL : FORWARD_VEL;

        const float rotTime = (2.0f * PI * radius) / (_isFast ? FAST_ROTATE_FORWARD_VEL : FORWARD_VEL) * 0.25;

        const float forwardTime = _isFixed ? (CELL_SIZE * 0.21f / (_isFast ? FAST_ROTATE_FORWARD_VEL : FORWARD_VEL)) : 0.0f;

        if (forwardTime > sensor->time)
        {
            callibrateRot(sensor, motorState);
            motorState->isComplited = false;
        }
        else if (rotTime * (_isFast ? FAST_ROTATE_COLLIBREATE_K : ROTATE_COLLIBREATE_K) + forwardTime > sensor->time)
        {
            motorState->headingVelocity = headingVel;
        }
        else if (rotTime * (_isFast ? FAST_ROTATE_COLLIBREATE_K : ROTATE_COLLIBREATE_K) + 2.0f * forwardTime > sensor->time)
        {
            callibrateRot(sensor, motorState);
            motorState->isComplited = false;
        }
        else
            motorState->isComplited = true;
    }
};

Rotate90 ROTATE_90_RIGHT(false, false, false);
Rotate90 ROTATE_90_LEFT(false, true, false);
Rotate90 ROTATE_90_RIGHT_FIX(false, false, true);
Rotate90 ROTATE_90_LEFT_FIX(false, true, true);
Rotate90 ROTATE_90_RIGHT_FAST(true, false, false);
Rotate90 ROTATE_90_LEFT_FAST(true, true, false);
Rotate90 ROTATE_90_RIGHT_FIX_FAST(true, false, true);
Rotate90 ROTATE_90_LEFT_FIX_FAST(true, true, true);

class Rotate45 : public ICyclogram
{
private:
    bool _side;
    bool _isRevers;

public:
    Rotate45(bool side, bool revers)
    {
        _side = side;
        _isRevers = revers;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        float a = ROTATE_45_RADIUS / sqrtf(2.0f);

        float b = (CELL_SIZE / 2.0f) - ROTATE_45_RADIUS + a;

        float firstForwardTime = (CELL_SIZE - b - a) / FAST_FORWARD_VEL * ROTATE_45_FIRST_FORWARD_K;
        float secondForwardTime = (b * sqrtf(2.0f)) / FAST_FORWARD_VEL;
        float rotTime = (2.0f * PI * ROTATE_45_RADIUS) / FAST_FORWARD_VEL * 0.125;

        if (sensor->time < (_isRevers ? secondForwardTime : firstForwardTime))
        {
            motorState->forwardVel = FAST_FORWARD_VEL;

            if (_isRevers)
                motorState->headingVelocity = 0.0f;
            else
                callibrateRot(sensor, motorState);

            motorState->isComplited = false;
        }
        else if (sensor->time < (_isRevers ? secondForwardTime : firstForwardTime) + rotTime)
        {
            motorState->forwardVel = FAST_FORWARD_VEL;
            motorState->headingVelocity = (_side ? 1.0f : -1.0f) * FAST_FORWARD_VEL / ROTATE_45_RADIUS;
        }
        else if (sensor->time < firstForwardTime + rotTime + secondForwardTime)
        {
            motorState->forwardVel = FAST_FORWARD_VEL;

            if (!_isRevers)
                motorState->headingVelocity = 0.0f;
            else
                callibrateRot(sensor, motorState);

            motorState->isComplited = false;
        }
        else
            motorState->isComplited = true;
    }
};

Rotate45 ROTATE_45_RIGHT(false, false);
Rotate45 ROTATE_45_LEFT(true, false);
Rotate45 ROTATE_45_RIGHT_REVERS(false, true);
Rotate45 ROTATE_45_LEFT_REVERS(true, true);

class Rotate135 : public ICyclogram
{
private:
    bool _side;
    bool _isRevers;

public:
    Rotate135(bool side, bool revers)
    {
        _side = side;
        _isRevers = revers;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        float a = ROTATE_135_RADIUS / tanf(22.5f);
        float b = CELL_SIZE / tanf(45.0f) - a;

        float firstForwardTime = ((CELL_SIZE / 2.0f) + b) / FAST_FORWARD_VEL * ROTATE_135_FIRST_FORWARD_K;
        float secondForwardTime = (CELL_SIZE / sinf(45.0f) - a) / FAST_FORWARD_VEL;
        float rotTime = (2.0f * PI * ROTATE_135_RADIUS) / FAST_ROTATE_FORWARD_VEL * (0.25f + 0.125f);

        if (sensor->time < (_isRevers ? secondForwardTime : firstForwardTime))
        {
            motorState->forwardVel = FAST_FORWARD_VEL;

            if (_isRevers)
                motorState->headingVelocity = 0.0f;
            else
                callibrateRot(sensor, motorState);

            motorState->isComplited = false;
        }
        else if (sensor->time < (_isRevers ? secondForwardTime : firstForwardTime) + rotTime)
        {
            motorState->forwardVel = FAST_ROTATE_FORWARD_VEL;
            motorState->headingVelocity = (_side ? 1.0f : -1.0f) * FAST_ROTATE_FORWARD_VEL / ROTATE_135_RADIUS;
            motorState->isComplited = false;
        }
        else if (sensor->time < firstForwardTime + secondForwardTime + rotTime)
        {
            motorState->forwardVel = FAST_FORWARD_VEL;

            if (!_isRevers)
                motorState->headingVelocity = 0.0f;
            else
                callibrateRot(sensor, motorState);

            motorState->isComplited = false;
        }
        else
            motorState->isComplited = true;
    }
};

Rotate135 ROTATE_135_LEFT(true, false);
Rotate135 ROTATE_135_RIGHT(false, false);
Rotate135 ROTATE_135_LEFT_REVERS(true, true);
Rotate135 ROTATE_135_RIGHT_REVERS(false, true);

class Diagnal90 : public ICyclogram
{
private:
    bool _side;

public:
    Diagnal90(bool side)
    {
        _side = side;
    }

    void run(Sensor *sensor, MotorState *motorState) override
    {
        const float radius = CELL_SIZE / sqrtf(2.0f);

        const float headingVel = (_side ? 1.0f : -1.0f) * FAST_FORWARD_VEL / radius;
        motorState->forwardVel = FAST_FORWARD_VEL;

        const float rotTime = (2.0f * PI * radius) / FAST_FORWARD_VEL * 0.25;

        if (rotTime * ROTATE_COLLIBREATE_K > sensor->time)
        {
            motorState->headingVelocity = headingVel;
            motorState->isComplited = false;
        }
        else
            motorState->isComplited = true;
    }
};

Diagnal90 DIAGONAL_90_LEFT(true);
Diagnal90 DIAGONAL_90_RIGHT(false);

class Rotate180Place : public ICyclogram
{
public:
    void run(Sensor *sensor, MotorState *motorState) override
    {
        float rotTime = PI / ROTATE_VEL * ROTATE180_COLLIBREATE_K;

        if (sensor->time < rotTime)
        {
            motorState->headingVelocity = ROTATE_VEL;
            motorState->forwardVel = 0.0f;
            motorState->isComplited = false;
        }
        else
            motorState->isComplited = true;
    }
};

Rotate180Place ROTATE_180_ON_PLACE;

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

    _cyclograms[0]->run(&sensor, &motorState);

    setDriveVelocity(motorState.forwardVel, motorState.headingVelocity);

    if (motorState.isComplited)
    {
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