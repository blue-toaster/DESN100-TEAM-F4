#include <Arduino.h>

// define pins
const int motor_l_speed_pin = 0;
const int motor_r_speed_pin = 0;
const int motor_l_forward_pin = 0;
const int motor_l_reverse_pin = 0;
const int motor_r_forward_pin = 0;
const int motor_r_reverse_pin = 0;

// define motor state
bool motor_l_speed = 0;
bool motor_r_speed = 0;
bool motor_l_dir = 0;
bool motor_r_dir = 0;
uint8_t loop_tracker = 0b0;

void setup() {
    // setup pin modes
    pinMode(motor_l_speed_pin, OUTPUT);
    pinMode(motor_r_speed_pin, OUTPUT);
    pinMode(motor_l_forward_pin, INPUT);
    pinMode(motor_l_reverse_pin, INPUT);
    pinMode(motor_r_forward_pin, INPUT);
    pinMode(motor_r_reverse_pin, INPUT);
    
}

void loop() {
    // loop motors spinning in all directions

    loop_tracker = (loop_tracker + 1) % 16;
    motor_l_speed = (loop_tracker) & 0b1;
    motor_r_speed = (loop_tracker >> 1) & 0b1;
    motor_l_dir = (loop_tracker >> 2) & 0b1;
    motor_r_dir = (loop_tracker >> 3) & 0b1;

    // set motor values
    digitalWrite(motor_l_forward_pin, motor_l_dir);
    digitalWrite(motor_l_reverse_pin, !motor_l_dir);
    digitalWrite(motor_r_forward_pin, motor_l_dir);
    digitalWrite(motor_r_reverse_pin, !motor_l_dir);

    analogWrite(motor_l_speed_pin, 255 * motor_l_speed);
    analogWrite(motor_r_speed_pin, 255 * motor_r_speed);

    delay(300);
}
