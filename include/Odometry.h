#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"
#include "RobotState.h"
#include "VelEstimator.h"

RobotState gRobotState;

void odometryInit(){
    gRobotState.x = 0.0f;
    gRobotState.y = 0.0f;
}

void odometryTick(){
    float lHeading = gLeftHeading, rHeading = gRightHeading;

    static float lHeadingOld = 0, rHeadingOld = 0;

    float Sl = (lHeading - lHeadingOld) * WHEEL_RADIUS;
    float Sr = (rHeading - rHeadingOld) * WHEEL_RADIUS;

    lHeadingOld = lHeading;
    rHeadingOld = rHeading;

    float Sf = (Sl + Sr) * 0.5;

    float dH = (Sr - Sl) / ROBOT_WIDTH;

    float medivalH = gRobotState.h + dH * 0.5;

    gRobotState.x += Sf * cos(medivalH);
    gRobotState.y += Sf * sin(medivalH);

    gRobotState.h += dH;
}