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

void Forward(Sensor *sensor, MotorState *motorState)
{
    motorState->forwardVel = FORWARD_VEL;

    callibrateRot(sensor, motorState);

    if (sensor->time > CELL_SIZE / FORWARD_VEL * FORWARD_COLLIBREATE_K ||
        max(sensor->distanceFrontLeft, sensor->distanceFrontRight) > FORWARD_RIDE_TRIGGER_DIST)
        motorState->isComplited = true;
}

void Rotate180(Sensor *sensor, MotorState *motorState)
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

void Idle(Sensor *sensor, MotorState *motorState)
{
    motorState->forwardVel = 0.0;
    motorState->headingVelocity = 0.0f;
    motorState->isComplited = false;
}

void Start(Sensor *sensor, MotorState *motorState)
{
    float forwardTime = (CELL_SIZE - ROBOT_WHHEL_DIF) / FORWARD_VEL;

    if (sensor->time < forwardTime + 0.4f)
    {
        motorState->forwardVel = -FORWARD_VEL;
        motorState->headingVelocity = 0.0f;

        callibrateRot(sensor, motorState);
    }
    else if (sensor->time < forwardTime * 2.0f + 0.4f)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;

        callibrateRot(sensor, motorState);
    }
    else
        motorState->isComplited = true;
}

void Stop(Sensor *sensor, MotorState *motorState)
{
    motorState->forwardVel = 0.0;
    motorState->headingVelocity = 0.0f;
    motorState->isComplited = true;
}

void Rotate90Left(Sensor *sensor, MotorState *motorState)
{
    const float radius = (CELL_SIZE * 0.49f);

    const float headingVel = 1.0 * (FORWARD_VEL / radius);
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    if (rotTime * ROTATE_COLLIBREATE_K > sensor->time)
    {
        motorState->headingVelocity = headingVel;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate90Right(Sensor *sensor, MotorState *motorState)
{
    const float radius = (CELL_SIZE * 0.49f);

    const float headingVel = -1.0 * (FORWARD_VEL / radius);
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    if (rotTime * ROTATE_COLLIBREATE_K > sensor->time)
    {
        motorState->headingVelocity = headingVel;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate90RightFix(Sensor *sensor, MotorState *motorState)
{
    const float radius = (CELL_SIZE * 0.29f);

    const float headingVel = -1.0 * (FORWARD_VEL / radius);
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    const float forwardTime = CELL_SIZE * 0.21f / FORWARD_VEL;

    if (forwardTime > sensor->time)
    {
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (rotTime * ROTATE_COLLIBREATE_K + forwardTime > sensor->time)
    {
        motorState->headingVelocity = headingVel;
        motorState->isComplited = false;
    }
    else if (rotTime * ROTATE_COLLIBREATE_K + 2.0f * forwardTime > sensor->time)
    {
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate90LeftFix(Sensor *sensor, MotorState *motorState)
{
    const float radius = (CELL_SIZE * 0.29f);

    const float headingVel = (FORWARD_VEL / radius);
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    const float forwardTime = CELL_SIZE * 0.21f / FORWARD_VEL;

    if (forwardTime > sensor->time)
    {
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (rotTime * ROTATE_COLLIBREATE_K + forwardTime > sensor->time)
    {
        motorState->headingVelocity = headingVel;
    }
    else if (rotTime * ROTATE_COLLIBREATE_K + 2.0f * forwardTime > sensor->time)
    {
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void HalfForward(Sensor *sensor, MotorState *motorState)
{
    motorState->forwardVel = FORWARD_VEL;

    callibrateRot(sensor, motorState);

    if (sensor->time > CELL_SIZE / FORWARD_VEL * FORWARD_COLLIBREATE_K * 0.5f)
        motorState->isComplited = true;
}

void Rotate180OnPlace(Sensor *sensor, MotorState *motorState)
{
    if (sensor->time < PI / ROTATE_VEL * ROTATE180_COLLIBREATE_K)
    {
        motorState->headingVelocity = ROTATE_VEL;
        motorState->forwardVel = 0.0;

        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate45Right(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_45_RADIUS / sqrtf(2.0f);

    float b = (CELL_SIZE / 2.0f) - ROTATE_45_RADIUS + a;

    float firstForwardTime = (CELL_SIZE - b - a) / FORWARD_VEL * ROTATE_45_FIRST_FORWARD_K;
    float secondForwardTime = (b * sqrtf(2.0f)) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_45_RADIUS) / FORWARD_VEL * 0.125;

    if (sensor->time < firstForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = -FORWARD_VEL / ROTATE_45_RADIUS;
    }
    else if (sensor->time < firstForwardTime + rotTime + secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate45Left(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_45_RADIUS / sqrtf(2.0f);

    float b = (CELL_SIZE / 2.0f) - ROTATE_45_RADIUS + a;

    float firstForwardTime = (CELL_SIZE - b - a) / FORWARD_VEL * ROTATE_45_FIRST_FORWARD_K;
    float secondForwardTime = (b * sqrtf(2.0f)) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_45_RADIUS) / FORWARD_VEL * 0.125;

    if (sensor->time < firstForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = FORWARD_VEL / ROTATE_45_RADIUS;
    }
    else if (sensor->time < firstForwardTime + rotTime + secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate45LeftRevers(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_45_RADIUS / sqrtf(2.0f);

    float b = (CELL_SIZE / 2.0f) - ROTATE_45_RADIUS + a;

    float firstForwardTime = (CELL_SIZE - b - a) / FORWARD_VEL * ROTATE_45_FIRST_FORWARD_K;
    float secondForwardTime = (b * sqrtf(2.0f)) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_45_RADIUS) / FORWARD_VEL * 0.125;

    if (sensor->time < secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else if (sensor->time < secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = FORWARD_VEL / ROTATE_45_RADIUS;
    }
    else if (sensor->time < firstForwardTime + rotTime + secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate45RightRevers(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_45_RADIUS / sqrtf(2.0f);

    float b = (CELL_SIZE / 2.0f) - ROTATE_45_RADIUS + a;

    float firstForwardTime = (CELL_SIZE - b - a) / FORWARD_VEL * ROTATE_45_FIRST_FORWARD_K;
    float secondForwardTime = (b * sqrtf(2.0f)) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_45_RADIUS) / FORWARD_VEL * 0.125;

    if (sensor->time < secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else if (sensor->time < secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = -FORWARD_VEL / ROTATE_45_RADIUS;
    }
    else if (sensor->time < firstForwardTime + rotTime + secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Forward45(Sensor *sensor, MotorState *motorState)
{
    float forwardDistance = (CELL_SIZE * 0.5f) * sqrtf(2.0f);

    if (sensor->time < forwardDistance / FORWARD_VEL)
    {
        motorState->headingVelocity = 0.0f;
        motorState->forwardVel = FORWARD_VEL;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void StartCenter(Sensor *sensor, MotorState *motorState)
{
    float forwardTime = (CELL_SIZE / 2.0f - ROBOT_WHHEL_DIF) / FORWARD_VEL;

    if (sensor->time < forwardTime + 0.5f)
    {
        motorState->forwardVel = -FORWARD_VEL;
        callibrateRot(sensor, motorState);

        motorState->isComplited = false;
    }
    else if (sensor->time < forwardTime * 2.0f + 0.5f)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);

        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate135Left(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_135_RADIUS / tanf(22.5f);
    float b = CELL_SIZE / tanf(45.0f) - a;

    float firstForwardTime = ((CELL_SIZE / 2.0f) + b) / FORWARD_VEL * ROTATE_135_FIRST_FORWARD_K;
    float secondForwardTime = (CELL_SIZE / sinf(45.0f) - a) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_135_RADIUS) / FORWARD_VEL * 0.375;

    if (sensor->time < firstForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = FORWARD_VEL / ROTATE_135_RADIUS;
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate135Right(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_135_RADIUS / tanf(22.5f);
    float b = CELL_SIZE / tanf(45.0f) - a;

    float firstForwardTime = ((CELL_SIZE / 2.0f) + b) / FORWARD_VEL * ROTATE_135_FIRST_FORWARD_K;
    float secondForwardTime = (CELL_SIZE / sinf(45.0f) - a) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_135_RADIUS) / FORWARD_VEL * 0.375;

    if (sensor->time < firstForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = -FORWARD_VEL / ROTATE_135_RADIUS;
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate135RightRevers(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_135_RADIUS / tanf(22.5f);
    float b = CELL_SIZE / tanf(45.0f) - a;

    float firstForwardTime = ((CELL_SIZE / 2.0f) + b) / FORWARD_VEL * ROTATE_135_FIRST_FORWARD_K;
    float secondForwardTime = (CELL_SIZE / sinf(45.0f) - a) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_135_RADIUS) / FORWARD_VEL * 0.375;

    if (sensor->time < secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else if (sensor->time < secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = -FORWARD_VEL / ROTATE_135_RADIUS;
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Rotate135LeftRevers(Sensor *sensor, MotorState *motorState)
{
    float a = ROTATE_135_RADIUS / tanf(22.5f);
    float b = CELL_SIZE / tanf(45.0f) - a;

    float firstForwardTime = ((CELL_SIZE / 2.0f) + b) / FORWARD_VEL * ROTATE_135_FIRST_FORWARD_K;
    float secondForwardTime = (CELL_SIZE / sinf(45.0f) - a) / FORWARD_VEL;
    float rotTime = (2.0f * PI * ROTATE_135_RADIUS) / FORWARD_VEL * 0.375;

    if (sensor->time < secondForwardTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = 0.0f;
        motorState->isComplited = false;
    }
    else if (sensor->time < secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        motorState->headingVelocity = FORWARD_VEL / ROTATE_135_RADIUS;
        motorState->isComplited = false;
    }
    else if (sensor->time < firstForwardTime + secondForwardTime + rotTime)
    {
        motorState->forwardVel = FORWARD_VEL;
        callibrateRot(sensor, motorState);
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Diagonal90Left(Sensor *sensor, MotorState *motorState){
    const float radius = CELL_SIZE / sqrtf(2.0f);

    const float headingVel = FORWARD_VEL / radius;
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    if (rotTime * ROTATE_COLLIBREATE_K > sensor->time)
    {
        motorState->headingVelocity = headingVel;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

void Diagonal90Right(Sensor *sensor, MotorState *motorState){
    const float radius = CELL_SIZE / sqrtf(2.0f);

    const float headingVel = FORWARD_VEL / radius;
    motorState->forwardVel = FORWARD_VEL;

    const float rotTime = (2.0f * PI * radius) / FORWARD_VEL * 0.25;

    if (rotTime * ROTATE_COLLIBREATE_K > sensor->time)
    {
        motorState->headingVelocity = -headingVel;
        motorState->isComplited = false;
    }
    else
        motorState->isComplited = true;
}

Vector<void (*)(Sensor *, MotorState *)> _cyclograms;
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

    _cyclograms[0](&sensor, &motorState);

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

void addCyclogramToQueue(void (*cyc)(Sensor *, MotorState *))
{
    _cyclograms.push_back(cyc);
}

void runCyclogram(void (*cyc)(Sensor *, MotorState *))
{
    _cyclograms.clear();

    addCyclogramToQueue(cyc);
}