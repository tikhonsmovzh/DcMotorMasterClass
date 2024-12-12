#pragma once

#include <Arduino.h>
#include "Config.h"

float gVoltmeterVolts = 1.0;

float decodeVoltmeter(int rawVolt){
    return rawVolt * ANALOG_TO_VOLTS;
}

void voltInit(){
    pinMode(VOLTMETER_PIN, INPUT);
}

void voltTick(){
    gVoltmeterVolts = decodeVoltmeter(analogRead(VOLTMETER_PIN));
}