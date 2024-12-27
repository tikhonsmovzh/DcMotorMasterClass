#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Motor.h"

void setDriveVelocity(float forwardVel, float headingVel){
    float W = forwardVel / WHEEL_RADIUS;

    const float deltaW = headingVel * HEADING_VEL_TO_DELTA_W;

    gTargetLeftW = W - deltaW;
    gTargetRightW = W + deltaW;
}