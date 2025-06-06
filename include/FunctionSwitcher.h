#pragma once

#include <Arduino.h>
#include "Config.h"
#include "ElapsedTime.h"

const int _functionRanges[] = FUNCTION_RANGES;

int decodeFunction(int rawValue)
{
  if (rawValue > 1000)
    return 16;

  for (int i = 0; i < 16; i++)
  {
    if (rawValue > (_functionRanges[i] + _functionRanges[i + 1]) / 2)
      return i;
  }

  return -1;
}

void functionInit()
{
  pinMode(FUNCTION_PIN, INPUT);
}

int gCurrentFunction = 0;
bool gStartButtonPresed = false;

int _functionBuffer[3]{0};
int _functionBufferSize = 3;

ElapseTime _buttonTimer;

void functionTick()
{
  int function = decodeFunction(analogRead(FUNCTION_PIN));

  // for (int i = 0; i < _functionBufferSize - 1; i++)
  //   _functionBuffer[i] = _functionBuffer[i + 1];

  // _functionBuffer[_functionBufferSize - 1] = function;

  // int min = 16, max = 0;

  // for (int i = 0; i < _functionBufferSize; i++)
  // {
  //   if (_functionBuffer[i] < min)
  //     min = _functionBuffer[i];

  //   if (_functionBuffer[i] > max)
  //     max = _functionBuffer[i];
  // }

  // int median = -1;

  // for (int i = 0; i < _functionBufferSize; i++)
  // {
  //   if (_functionBuffer[i] != min && _functionBuffer[i] != max)
  //   {
  //     median = _functionBuffer[i];

  //     break;
  //   }
  // }

  // if (median == -1)
  //   median = max;

  if (function != 16)
  {
    gCurrentFunction = function;

    gStartButtonPresed = false;
    _buttonTimer.reset();
  }
  else{
    if(_buttonTimer.seconds() > START_BUTTON_TRIGGER_TME)
      gStartButtonPresed = true;
    else
      gStartButtonPresed = false;
  }
}