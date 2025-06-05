#pragma once

#include <Arduino.h>
#include <Config.h>

class PIRegulator{
private:
    float _kP, _kI, _maxU, _integrall = 0.0f;

public:
    PIRegulator(float kP, float kI, float maxU){
        _kP = kP;
        _kI = kI;
        _maxU = maxU;
    }

    float update(float err){
        const float uP = _kP * err;
        const float uI = _kI * _integrall;

        const float u = uP + uI;

        if((u < _maxU && u > -_maxU) || (err * u < 0.0f))
            _integrall += err * Ts_s;

        return u;
    }

    void setMaxU(float maxU){
        _maxU = maxU;
    }
};