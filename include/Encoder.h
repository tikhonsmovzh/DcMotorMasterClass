#pragma once

#include <Arduino.h>
#include "Config.h"

volatile int _leftTicksCounter = 0;
float gLeftHeading = 0;

volatile int _rightTicksCounter = 0;
float gRightHeading = 0;

int8_t _ett[4][4] = {0};

void leftEncHandler(){
    static uint8_t leftEncOld = 0;

    const uint8_t leftB = digitalRead(LEFT_B_PIN);
    const uint8_t leftClkA = digitalRead(LEFT_CLK_A_PIN);
    const uint8_t leftA = leftClkA ^ leftB;

    const uint8_t leftEnc = (leftA << 1) | leftB;

    _leftTicksCounter += _ett[leftEncOld][leftEnc] * LEFT_ENC_DIR;

    leftEncOld = leftEnc;
}

void rightEncHandler(){
    static uint8_t rightEncOld = 0;

    const uint8_t rightB = digitalRead(RIGHT_B_PIN);
    
    const uint8_t rightClkA = digitalRead(RIGHT_CLK_A_PIN);
    const uint8_t rightA = rightClkA ^ rightB;

    const uint8_t rightEnc = (rightA << 1) | rightB;

    _rightTicksCounter += /*_ett[rightEncOld][rightEnc]**/ RIGHT_ENC_DIR;

    rightEncOld = rightEnc;
}

void leftEncTick(){
    noInterrupts();

    const int leftCounterInc = _leftTicksCounter;
    _leftTicksCounter = 0;

    interrupts();

    gLeftHeading += leftCounterInc * TICKS_TO_RAD;
}

void rightEncTick(){
    noInterrupts();

    const int rightCounterInc = _rightTicksCounter;
    _rightTicksCounter = 0;

    interrupts();

    gRightHeading += rightCounterInc * TICKS_TO_RAD;
}

void encsInit(){
    pinMode(LEFT_CLK_A_PIN, INPUT);
    pinMode(LEFT_B_PIN, INPUT);

    pinMode(RIGHT_CLK_A_PIN, INPUT);
    pinMode(RIGHT_B_PIN, INPUT);

    noInterrupts();

    attachInterrupt(digitalPinToInterrupt(LEFT_CLK_A_PIN), leftEncHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_CLK_A_PIN), rightEncHandler, CHANGE);

    _ett[0b00][0b01] = 1;
    _ett[0b01][0b11] = 1;
    _ett[0b11][0b10] = 1;
    _ett[0b10][0b00] = 1;

    _ett[0b00][0b10] = -1;
    _ett[0b10][0b11] = -1;
    _ett[0b11][0b01] = -1;
    _ett[0b01][0b00] = -1;

    interrupts();
}