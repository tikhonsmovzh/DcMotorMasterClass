#pragma once

#include <Arduino.h>
#include "Config.h"

void analogueSetup()
{
    bitClear(ADCSRA, ADPS0);
    bitClear(ADCSRA, ADPS1);
    bitSet(ADCSRA, ADPS2);
}

void setupSystick()
{
    bitClear(TCCR2A, WGM20);
    bitSet(TCCR2A, WGM21);
    bitClear(TCCR2B, WGM22);
    bitSet(TCCR2B, CS22);
    bitClear(TCCR2B, CS21);
    bitSet(TCCR2B, CS20);
    OCR2A = 249;
    bitSet(TIMSK2, OCIE2A);
}

volatile int16_t _sensorFrontLeft = 0, _sensorFrontRight = 0,
                 _sensorDiagonalLeft = 0, _sensorDiagonalRight = 0;

int16_t gDistanceFrontLeft = 0, gDistanceFrontRight = 0,
        gDistanceDiagonalLeft = 0, gDistanceDiagonalRight = 0;

void distanceSensorsInit()
{
    pinMode(EMITTER_FRONT_PIN, OUTPUT);
    pinMode(EMITTER_DIAGONAL_PIN, OUTPUT);  

    analogueSetup();
    setupSystick();
}

void distanceSensorsTick(){
    gDistanceFrontLeft = _sensorFrontLeft;
    gDistanceFrontRight = _sensorFrontRight;
    gDistanceDiagonalLeft = _sensorDiagonalLeft;
    gDistanceDiagonalRight = _sensorDiagonalRight;
}

volatile bool _lastReadSensor = false;

void distanceSensorsISRTick(){
    _lastReadSensor = !_lastReadSensor;

    if (_lastReadSensor) {
        uint16_t darkLeftFront = analogRead(TRIGGER_FRONT_LEFT_PIN);
        uint16_t darkRightFront = analogRead(TRIGGER_FRONT_RIGHT_PIN);
    
        digitalWrite(EMITTER_FRONT_PIN, 1);
        delayMicroseconds(LED_ENABLE_TIME_MICROS);
    
        uint16_t leftFront = analogRead(TRIGGER_FRONT_LEFT_PIN);
        uint16_t rightFront = analogRead(TRIGGER_FRONT_RIGHT_PIN);
    
        digitalWrite(EMITTER_FRONT_PIN, 0);
    
        _sensorFrontLeft = leftFront - darkLeftFront;
        _sensorFrontRight = rightFront - darkRightFront;
    }
    else{   
        uint16_t darkLeftDiagonal = analogRead(TRIGGER_DIAGONAL_LEFT_PIN);
        uint16_t darkRightDiagonal = analogRead(TRIGGER_DIAGONAL_RIGHT_PIN);
    
        digitalWrite(EMITTER_DIAGONAL_PIN, 1);
        delayMicroseconds(LED_ENABLE_TIME_MICROS);
    
        uint16_t leftDiagonal = analogRead(TRIGGER_DIAGONAL_LEFT_PIN);
        uint16_t rightDiagonal = analogRead(TRIGGER_DIAGONAL_RIGHT_PIN);
    
        digitalWrite(EMITTER_DIAGONAL_PIN, 0);
    
        _sensorDiagonalLeft = leftDiagonal - darkLeftDiagonal;
        _sensorDiagonalRight = rightDiagonal - darkRightDiagonal;
    }
}

ISR(TIMER2_COMPA_vect) {
    distanceSensorsISRTick();
}

bool isWallForward()
{
    return gDistanceFrontLeft > FORWARD_WALL_TRIGGER_DISTANCE &&
           gDistanceFrontRight > FORWARD_WALL_TRIGGER_DISTANCE;
}

bool isWallLeft()
{
    return gDistanceDiagonalLeft > DIAGONAL_WALL_TRIGGER_LEFT_DISTANCE;
}

bool isWallRight()
{
    return gDistanceDiagonalRight > DIAGONAL_WALL_TRIGGER_RIGHT_DISTANCE;
}
