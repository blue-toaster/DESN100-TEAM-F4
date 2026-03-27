#include "motor_tests/motor_test1.hpp"



void motorTest::init() {
    // setup pin modes
    pinMode(motor_l_speed_pin, OUTPUT);
    pinMode(motor_r_speed_pin, OUTPUT);
    pinMode(motor_l_direction_pin, INPUT);
    pinMode(motor_r_direction_pin, INPUT);
    
}

void motorTest::run() {
    // loop motors spinning in all directions


    // set motor values
    digitalWrite(motor_l_direction_pin, 1);
    digitalWrite(motor_r_direction_pin, 1);

    analogWrite(motor_l_speed_pin, 255);
    analogWrite(motor_r_speed_pin, 255);

    delay(300);
}
