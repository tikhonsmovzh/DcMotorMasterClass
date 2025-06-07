#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Encoder.h"

float leftVelocityEstimator(float heading){
    static float headingOld = 0;

    float wRaw = (heading - headingOld) / Ts_s;

    headingOld = heading;

    return wRaw;
}

float leftFilter(float wRaw){
    static float w = 0;

    w += (wRaw - w) * LPF_ALPHA;

    return  w;
}

float rightFilter(float wRaw){
    static float w = 0;

    w += (wRaw - w) * LPF_ALPHA;

    return w;
}

float rightVelocityEstimator(float heading){
    static float headingOld = 0;

    float wRaw = (heading - headingOld) / Ts_s;
    
    headingOld = heading;

    return wRaw;
}

float gLeftW = 0;
float gRightW = 0;

void velocityTick(){
    const float leftHeading = gLeftHeading;
    const float rightHeading = gRightHeading;

    const float leftWRaw = leftVelocityEstimator(leftHeading);
    const float rightWRaw = rightVelocityEstimator(rightHeading);

    gLeftW = leftFilter(leftWRaw);
    gRightW = rightFilter(rightWRaw);
}