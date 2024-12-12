#pragma once

#include <Arduino.h>
#include "Config.h"

const int _functionRanges[] = FUNCTION_RANGES;

int decodeFunction(int rawValue){
  if(rawValue > 1000)
    return 16;

  for (int i = 0; i < 16; i++) {
    if (rawValue > (_functionRanges[i] + _functionRanges[i + 1]) / 2)
      return i;
  }

  return -1;
}

void functionInit(){
    pinMode(FUNCTION_PIN, INPUT);
}

int gCurrentFunction = 0;

void functionTick(){
    gCurrentFunction = decodeFunction(analogRead(FUNCTION_PIN));
}