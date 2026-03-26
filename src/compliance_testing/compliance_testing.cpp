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

    // PD controller init
    controller.setCoefficients(K_p, K_d);
}

void complianceTesting::run() {
    // get blocks in frame
    pixy.ccc.getBlocks();

    if (target_index) {
        w_cur = pixy.ccc.blocks[target_index].m_width;
        h_cur = pixy.ccc.blocks[target_index].m_height;
    }

    long obsticle_detection = readUltrasonic(ultrasonic_trigger_pin, ultrasonic_echo_pin);

    if (obsticle_detection < ultrasonic_obsticle_thr) {
        analogWrite(motor_l_speed_pin, 0);
        analogWrite(motor_r_speed_pin, 0);
        return;
    }

    switch (bot_state) {
        case BotState::DRONE_COL:
            performDrone();
        case BotState::COL_NAV:
            navigateCol();
        case BotState::COL_GRAB:
            delay(100);
            bot_state = BotState::DRONE_DEP;
            target_colour = BASE;
        case BotState::DRONE_DEP:
            performDrone();
        case BotState::DEP_NAV:
            navigateDep();
        case BotState::DEP_DROP:
            delay(100);
            bot_state = BotState::DRONE_COL;
            target_colour = 0;
        default:
            delay(100);
    }

    if (target_index) {
        age_prev = pixy.ccc.blocks[target_index].m_age;
        w_cur = pixy.ccc.blocks[target_index].m_width;
        h_prev = pixy.ccc.blocks[target_index].m_height;
    }
}

void complianceTesting::performDrone() {
    setMotors((300 / axle_length));

    // check for blocks in frame
    if (pixy.ccc.numBlocks) {
        for (int i = 0; i < pixy.ccc.numBlocks; i++) {
            if (pixy.ccc.blocks[i].m_signature == target_colour) {
                target_index = pixy.ccc.blocks[i].m_index;
                bot_state = BotState::COL_NAV;
                break;
            }
        }
    }
}

void complianceTesting::navigateCol() {
    float delta_scale = (w_cur * h_cur) / (w_prev * h_prev);
    delta_scale = abs(1 - delta_scale);

    if (
        delta_scale > bounding_box_thr && 
        abs(pixy.ccc.blocks[target_index].m_age - age_prev) > age_thr &&
        target_colour != pixy.ccc.blocks[target_index].m_signature
    ) {
        // lost targeting block
        bot_state = BotState::DRONE_COL;
        return;
    }
    
    // target object close enough?
    if (
        (pixy.ccc.blocks[target_index].m_y + (pixy.ccc.blocks[target_index].m_height / 2)) > y_closeness_thr && 
        pixy.ccc.blocks[target_index].m_age > age_thr
    ) {
        setMotors(0);
        delay((100 / (5 * PI * wheel_radius)) * 1000);
        analogWrite(motor_l_speed_pin, 0);
        analogWrite(motor_r_speed_pin, 0);
        delay(500);
        bot_state = BotState::COL_GRAB;
    }

    // get optimal rotation speed
    target_rotation_speed = controller.updatePD((long)(pixy.ccc.blocks[target_index].m_x - 157.5));

    // set motors
    setMotors(target_rotation_speed);
}

void complianceTesting::navigateDep() {
    float delta_scale = (w_cur * h_cur) / (w_prev * h_prev);
    delta_scale = abs(1 - delta_scale);

    if (
        delta_scale > bounding_box_thr && 
        abs(pixy.ccc.blocks[target_index].m_age - age_prev) > age_thr &&
        target_colour != pixy.ccc.blocks[target_index].m_signature
    ) {
        // lost targeting block
        bot_state = BotState::DRONE_COL;
        return;
    }
    
    // is base close enough
    if (
        (w_cur * h_cur) > base_size_thr && 
        pixy.ccc.blocks[target_index].m_age > age_thr
    ) {
        setMotors(0);
        analogWrite(motor_l_speed_pin, 0);
        analogWrite(motor_r_speed_pin, 0);
        delay(500);
        bot_state = BotState::DEP_DROP;
    }

    target_rotation_speed = controller.updatePD((long)(pixy.ccc.blocks[target_index].m_x - 157.5));

    setMotors(target_rotation_speed);
}
