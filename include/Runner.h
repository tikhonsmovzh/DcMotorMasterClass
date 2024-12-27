#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Odometry.h"
#include "DriveTrain.h"

RobotState gTragetRobotState;

void runnerTick(){
    const RobotState currentRobotState = gRobotState;

    const float xErr = gTragetRobotState.x - currentRobotState.x;
    const float yErr = gTragetRobotState.y - currentRobotState.y;

    const float lErr = sqrtf(xErr * xErr + yErr * yErr);

    float targetH = gTragetRobotState.h;

    if(abs(lErr) > L_SENS)
        targetH = atan2(yErr, xErr);

    const float hErr = targetH - currentRobotState.h;

    float lU = lErr * L_P;
    float hU = hErr * H_P;

    if(abs(hErr) > H_SENS)
        lU = 0.0;

    setDriveVelocity(lU, hU);
}