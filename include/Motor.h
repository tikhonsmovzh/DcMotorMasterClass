#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Voltmeter.h"
#include "PIRegulator.h"
#include "VelEstimator.h"

void setLeftU(float powerU){
    int pwm = 255 * constrain(powerU / gVoltmeterVolts, -1.0f, 1.0f);

    if(pwm * LEFT_MOTOR_DIR > 0)
        digitalWrite(LEFT_MOTOR_DIR_PIN, LOW);
    else
        digitalWrite(LEFT_MOTOR_DIR_PIN, HIGH);
    
    analogWrite(LEFT_MOTOR_PWM_PIN, abs(pwm));
}

void setRightU(float powerU){
    int pwm = 255 * constrain(powerU / gVoltmeterVolts, -1.0f, 1.0f);

    if(pwm * RIGHT_MOTOR_DIR > 0)
        digitalWrite(RIGHT_MOTOR_DIR_PIN, LOW);
    else
        digitalWrite(RIGHT_MOTOR_DIR_PIN, HIGH);
    
    analogWrite(RIGHT_MOTOR_PWM_PIN, abs(pwm));
}

void motorInit(){
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);

    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);
}

PIRegulator _leftRegulator(MOTOR_K_P, MOTOR_K_I, MAX_MOTOR_U);
PIRegulator _rightRegulator(MOTOR_K_P, MOTOR_K_I, MAX_MOTOR_U);

float gTargetLeftW = 0.0;
float gTargetRightW = 0.0;

void motorTick(){
    const float currentLeftW = gLeftW;
    const float currentRightW = gRightW;
    
    setRightU(_rightRegulator.update(gTargetRightW - currentRightW));
    setLeftU(_leftRegulator.update(gTargetLeftW - currentLeftW));
}