#include "compliance_testing/compliance_testing.hpp"

long complianceTesting::readUltrasonic(const int trig, const int echo) {
    // set trig 0
    digitalWrite(trig, LOW);

    // send pulse
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // return distance
    return (ultrasonic_multi_factor * pulseIn(echo, HIGH));
}

void complianceTesting::setMotors(long v_theta) {

    // set default direction
    R_dir = 1;
    L_dir = 1;

    // get maximum forward speed for given angular velocity
    long v_f = ((max_RPM / 60) * 2 * PI * axle_radius) - (abs(v_theta) * axle_radius);

    // set wheel speeds
    wheel_R_speed = ((v_f - v_theta * axle_radius) * 60) / (2 * PI * wheel_radius);
    wheel_R_speed = ((v_f + v_theta * axle_radius) * 60) / (2 * PI * wheel_radius);

    // get set correct directions
    if (wheel_R_speed < 0) {
        R_dir = 0;
        wheel_R_speed = abs(wheel_R_speed);
    }

    if (wheel_L_speed < 0) {
        L_dir = 0;
        wheel_L_speed = abs(wheel_L_speed);
    }
    
    // write to motors
    digitalWrite(motor_l_direction_pin, L_dir);
    digitalWrite(motor_r_direction_pin, R_dir);
    analogWrite(motor_l_speed_pin, map(wheel_L_speed, 0, 150, 0, 255));
    analogWrite(motor_r_speed_pin, map(wheel_R_speed, 0, 150, 0, 255));
}

void complianceTesting::init() {
    // motor init
    pinMode(motor_l_direction_pin, OUTPUT);
    pinMode(motor_r_direction_pin, OUTPUT);
    pinMode(motor_l_speed_pin, OUTPUT);
    pinMode(motor_r_speed_pin, OUTPUT);

    // ultrasonic init
    pinMode(ultrasonic_echo_pin, INPUT);
    pinMode(ultrasonic_trigger_pin, OUTPUT);

    // pixycam init
    pixy.init();
}

void complianceTesting::run() {

}
