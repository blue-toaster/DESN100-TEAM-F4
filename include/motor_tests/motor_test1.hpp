#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include "interfaces.hpp"

class motorTest : modesObject 
{
public:
    void run();
    void init();

private:
    // define pins
    const int motor_l_speed_pin = 5;
    const int motor_r_speed_pin = 6;
    const int motor_l_direction_pin = 4;
    const int motor_r_direction_pin = 7;

    // define motor state
    bool motor_l_speed = 0;
    bool motor_r_speed = 0;
    bool motor_l_dir = 0;
    bool motor_r_dir = 0;
    uint8_t loop_tracker = 0b0;

};

#endif
