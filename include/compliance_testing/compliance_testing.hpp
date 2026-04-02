#ifndef COMPLIANCE_TESTING_H
#define COMPLIANCE_TESTING_H

#include "interfaces.hpp"
#include "pd_controller.hpp"

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define BASE 5

enum class BotState {
    DRONE_COL,
    COL_NAV,
    COL_GRAB,
    DRONE_DEP,
    DEP_NAV,
    DEP_DROP
};

class complianceTesting : modesObject
{
public:
    // base arduino functionality
    void run();
    void init();

    // utility
    long readUltrasonic(const int trig, const int ehco);
    void setMotors(double v_theta, int move);
    Block getDetection(uint16_t index);
    bool checkBlock(Block block);

    // core functionality
    void performDrone();
    void navigateCol();
    void navigateDep();
    void grab();
    void release();

private:
    // pin definitions

    // motors
    const int motor_l_speed_pin = 5;
    const int motor_r_speed_pin = 6;
    const int motor_l_direction_pin = 4;
    const int motor_r_direction_pin = 7;

    // utlrasonic
    const int ultrasonic_trigger_pin = 8;
    const int ultrasonic_echo_pin = 2;
    const int buzzer_pin = 0;

    // pixycam
    Pixy2 pixy;

    // constants
    const double ultrasonic_multi_factor = 0.01723;
    const int distance_thr = 20;
    const int ultrasonic_range[2] = {2, 400};
    const int buzzer_range[2] = {1, 5};
    const double max_RPM = 150;
    const double wheel_radius = 3.5;
    const double axle_length = 16.5;
    double axle_radius = 0;
    const double bounding_box_thr = 0.2;
    const uint16_t age_thr = 60;
    const double ultrasonic_obsticle_thr = 10;
    const uint16_t base_sig = 5;
    const uint16_t y_closeness_thr = 200;
    const uint16_t base_size_thr = 500;
    int ball_colours[3] = {RED, GREEN, BLUE};
    int colour_counter = 0;

    // PD controller
    PDController controller;
    // fresh battery
    const double K_p = -0.75;
    const double K_d = 0.008;

    // motor Movement Variables
    double wheel_R_speed = 0;
    double wheel_L_speed = 0;
    double target_rotation_speed = 0;
    bool R_dir = 0;
    bool L_dir = 0;

    // functionality variables
    BotState bot_state = BotState::DRONE_COL;
    uint16_t target_colour = RED;
    uint16_t target_index;

    // pixy block tracking
    uint16_t w_prev;
    uint16_t h_prev;
    uint16_t w_cur;
    uint16_t h_cur;
    uint16_t age_prev;

    Block target_block;

};

#endif
