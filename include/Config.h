#pragma once

//timer
#define Ts_us 10000
#define Ts_s (Ts_us / 1000000.0f)

//encoders
#define LEFT_CLK_A_PIN 2
#define LEFT_B_PIN 4
#define RIGHT_CLK_A_PIN 3
#define RIGHT_B_PIN 5
#define LEFT_ENC_DIR -1 
#define RIGHT_ENC_DIR 1
#define ENC_PPR 48
#define GEAR_RATIO 30
#define TICKS_TO_RAD  (2.0f * PI / (ENC_PPR * GEAR_RATIO))

//vel estimator
#define LPF_ALPHA 0.45f

//odometry
#define WHEEL_RADIUS (0.033f / 2.0f)
#define ROBOT_WIDTH 0.074f

//functions switcher
#define FUNCTION_PIN A6
#define FUNCTION_RANGES {660, 647, 630, 614, 590, 570, 545, 522, 461, 429, 385, 343, 271, 212, 128, 44,  0}

//voltmeter
#define VOLTMETER_PIN 21
#define BATTERY_DRIVER_RATIO 2.0f
#define MAX_PIN_VOLTADGE 5.0f
#define ANALOG_TO_VOLTS ((MAX_PIN_VOLTADGE * BATTERY_DRIVER_RATIO) / 1023.0)

//motors
#define LEFT_MOTOR_DIR_PIN 7 
#define LEFT_MOTOR_PWM_PIN 9
#define LEFT_MOTOR_DIR -1
#define RIGHT_MOTOR_DIR 1
#define RIGHT_MOTOR_DIR_PIN 8 
#define RIGHT_MOTOR_PWM_PIN 10

#define USB_MOTOR_POWER 4.5

//velocity regulators
#define MAX_MOTOR_U 8.0f
#define MOTOR_K_P 0.95f
#define MOTOR_K_I 2.9f

//drive train
#define HEADING_VEL_TO_DELTA_W ((ROBOT_WIDTH / WHEEL_RADIUS) * 0.5f)

//cyclograms
#define FORWARD_VEL 0.18f
#define ROTATE_VEL 5.0f
#define CELL_SIZE 0.18f

#define FORWARD_CYCLOGRAM_P 0.006f
#define TARGET_FORWARD_DISTANCE_LEFT 40
#define TARGET_FORWARD_DISTANCE_RIGHT 25

#define ROTATE_COLLIBREATE_K 1.07f

#define FORWARD_COLLIBREATE_K 0.9f

#define ROTATE180_COLLIBREATE_K 1.05f

#define FORWARD_RIDE_TRIGGER_DIST 125
  
//Maze
#define MAZE_SIZE_X 10
#define MAZE_SIZE_Y 10

#define HORIZONTAL_WALL "---"
#define VERITICAL_WALL "|"

#define HORIZONTAL_WALL_ENPTY "   "
#define VERITICAL_WALL_EMPTY " "

#define HORIZONTAL_WALL_UNKNOWN " . "
#define VERITICAL_WALL_UNKNOWN "."

#define CORNER "+"

//Queue
#define MAX_QUEUE_SIZE (MAZE_SIZE_X * 2 + MAZE_SIZE_Y * 2)

//Distance sensors
#define EMITTER_FRONT_PIN 11
#define EMITTER_DIAGONAL_PIN 12

#define TRIGGER_FRONT_LEFT_PIN A3
#define TRIGGER_FRONT_RIGHT_PIN A0

#define TRIGGER_DIAGONAL_LEFT_PIN A2
#define TRIGGER_DIAGONAL_RIGHT_PIN A1

#define LED_ENABLE_TIME_MICROS 50

//Maze explorer
#define FORWARD_WALL_TRIGGER_DISTANCE 30
#define DIAGONAL_WALL_TRIGGER_RIGHT_DISTANCE 25
#define DIAGONAL_WALL_TRIGGER_LEFT_DISTANCE 40

#define H_SENS PI * 0.25