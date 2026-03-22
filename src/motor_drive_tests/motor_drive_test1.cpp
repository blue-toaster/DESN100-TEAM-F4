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

    loop_tracker = (loop_tracker + 1) % 16;
    motor_l_speed = (loop_tracker) & 0b1;
    motor_r_speed = (loop_tracker >> 1) & 0b1;
    motor_l_dir = (loop_tracker >> 2) & 0b1;
    motor_r_dir = (loop_tracker >> 3) & 0b1;

    // set motor values
    digitalWrite(motor_l_direction_pin, motor_l_dir);
    digitalWrite(motor_r_direction_pin, motor_l_dir);

    analogWrite(motor_l_speed_pin, 255 * motor_l_speed);
    analogWrite(motor_r_speed_pin, 255 * motor_r_speed);

    delay(300);
}
