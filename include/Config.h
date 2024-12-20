#pragma once

#define Ts_us 30000
#define Ts_s (Ts_us / 1000000.0)


#define LEFT_CLK_A_PIN 2
#define LEFT_B_PIN 4
#define RIGHT_CLK_A_PIN 3
#define RIGHT_B_PIN 5
#define LEFT_ENC_DIR -1
#define RIGHT_ENC_DIR 1
#define ENC_PPR 48
#define GEAR_RATIO 30
#define TICKS_TO_RAD  (2.0f * PI / (ENC_PPR * GEAR_RATIO))


#define LPF_ALPHA 0.5f


#define WHEEL_RADIUS (0.032f / 2) //32.8
#define ROBOT_WIDTH 0.07f //0.0695


#define FUNCTION_PIN A6
#define FUNCTION_RANGES {660, 647, 630, 614, 590, 570, 545, 522, 461, 429, 385, 343, 271, 212, 128, 44,  0}


#define VOLTMETER_PIN 26
#define BATTERY_DRIVER_RATIO 2.0
#define MAX_PIN_VOLTADGE 3.5f
#define ANALOG_TO_VOLTS ((MAX_PIN_VOLTADGE * BATTERY_DRIVER_RATIO) / 1023.0)


#define LEFT_MOTOR_DIR_PIN 7 
#define LEFT_MOTOR_PWM_PIN 9

#define LEFT_MOTOR_DIR -1
#define RIGHT_MOTOR_DIR 1
 
#define RIGHT_MOTOR_DIR_PIN 8 
#define RIGHT_MOTOR_PWM_PIN 10

#define MAX_MOTOR_U 4.2f

#define MOTOR_K_P 0.8f
#define MOTOR_K_I 1.8f