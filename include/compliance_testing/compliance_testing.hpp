#ifndef COMPLIANCE_TESTING_H
#define COMPLIANCE_TESTING_H

#include "interfaces.hpp"
#include "pd_controller.hpp"

enum class BotState {
    DRONE_COL,
    COL_NAV,
    COL_GRAB,
    DRONE_DEP,
    DEP_NAV,
    DEP_DROP
};

enum class ColourSignitures {
    RED,
    GREEN,
    BLUE,
    BASE
};

class complianceTesting : modesObject
{
public:
    // base arduino functionality
    void run();
    void init();

    // utility
    long readUltrasonic(const int trig, const int ehco);
    void setMotors(long v_theta);

    // core functionality
    void performDrone(BotState target_colour);
    void navigate();
    void grab();
    void release();

private:
    // pin definitions

    // motors
    const int motor_l_speed_pin = 0;
    const int motor_r_speed_pin = 0;
    const int motor_l_direction_pin = 0;
    const int motor_r_direction_pin = 0;

    // utlrasonic
    const int ultrasonic_trigger_pin = 0;
    const int ultrasonic_echo_pin = 0;
    const int buzzer_pin = 0;

    // pixycam
    Pixy2 pixy;

    // constants
    const float ultrasonic_multi_factor = 0.01723;
    const int distance_thr = 20;
    const int ultrasonic_range[2] = {2, 400};
    const int buzzer_range[2] = {1, 5};
    const long max_RPM = 150;
    const long wheel_radius = 0;
    const long axle_length = 0;
    const long axle_radius = 0;

    // PD controller
    PDController controller;

    // motor Movement Variables
    long wheel_R_speed = 0;
    long wheel_L_speed = 0;
    long target_forward_speed = 0;
    long target_rotation_speed = 0;
    long final_R_rpm = 0;
    long final_L_rpm = 0;
    bool R_dir = 0;
    bool L_dir = 0;

    // functionality variables
    BotState bot_state = BotState::DRONE_COL;
    ColourSignitures target_colour = ColourSignitures::RED;
};

#endif
