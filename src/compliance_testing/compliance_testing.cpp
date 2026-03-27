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

void complianceTesting::setMotors(double v_theta) {

    axle_radius = axle_length / 2;

    // set default direction
    R_dir = 0;
    L_dir = 0;

    // get maximum forward speed for given angular velocity
    long v_f = ((max_RPM / 60) * 2 * PI * wheel_radius) - (abs(v_theta) * axle_radius);

    // debug turning
    v_f = 0;

    // set wheel speeds
    wheel_L_speed = ((v_f - v_theta * axle_radius) * 60) / (2 * PI * wheel_radius);
    wheel_R_speed = ((v_f + v_theta * axle_radius) * 60) / (2 * PI * wheel_radius);

    Serial.print("Wheel speeds (rpm): ");
    Serial.print(wheel_L_speed);
    Serial.print(" : ");
    Serial.print(wheel_R_speed);
    Serial.print("\n");

    // get set correct directions
    if (wheel_R_speed < 0) {
        R_dir = 1;
        wheel_R_speed = abs(wheel_R_speed);
    }

    if (wheel_L_speed < 0) {
        L_dir = 1;
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

    Serial.begin(9600);
}

void complianceTesting::run() {
    // get blocks in frame
    pixy.ccc.getBlocks();

    if (target_index) {
        target_block = getDetection(target_index);
        w_cur = target_block.m_width;
        h_cur = target_block.m_height;
        Serial.print("Tracking: ");
        Serial.print(target_index);
        Serial.print("\n");
    }

    // long obsticle_detection = readUltrasonic(ultrasonic_trigger_pin, ultrasonic_echo_pin);

    // if (obsticle_detection < ultrasonic_obsticle_thr) {
    //     analogWrite(motor_l_speed_pin, 0);
    //     analogWrite(motor_r_speed_pin, 0);
    //     return;
    // }

    switch (bot_state) {
        case BotState::DRONE_COL:
            performDrone();
            break;
        case BotState::COL_NAV:
            navigateCol();
            break;
        case BotState::COL_GRAB:
            delay(100);
            bot_state = BotState::DRONE_DEP;
            target_colour = BASE;
            break;
        case BotState::DRONE_DEP:
            performDrone();
            break;
        case BotState::DEP_NAV:
            navigateDep();
            break;
        case BotState::DEP_DROP:
            delay(100);
            bot_state = BotState::DRONE_COL;
            target_colour = 0;
            break;
        default:
            delay(100);
            break;
    }

    if (target_index) {
        age_prev = target_block.m_age;
        w_cur = target_block.m_width;
        h_prev = target_block.m_height;
    }
}

void complianceTesting::performDrone() {
    Serial.print("Scanning zone\n");
    setMotors(0);

    // check for blocks in frame
    if (pixy.ccc.numBlocks) {
      Serial.print("Detected: ");
      Serial.print(pixy.ccc.numBlocks);
      Serial.print("\n");
        for (int i = 0; i < pixy.ccc.numBlocks; i++) {
          Serial.print(pixy.ccc.blocks[i].m_signature);
          Serial.print("\n");
            if (pixy.ccc.blocks[i].m_signature == target_colour) {
                target_index = pixy.ccc.blocks[i].m_index;
                w_cur = pixy.ccc.blocks[i].m_width;
                h_cur = pixy.ccc.blocks[i].m_height;
                bot_state = BotState::COL_NAV;
                Serial.print("Found object\n");
                break;
            }
        }
    }
}

void complianceTesting::navigateCol() {
    Serial.print("Navigating to object\n");
    
    if (!checkBlock(target_block)) {
        // lost targeting block
        Serial.print("Lost block\n");
        bot_state = BotState::DRONE_COL;
        target_index = 0;
        return;
    }

    Serial.print("Upperbound height: \n");
    Serial.print((target_block.m_y - (uint16_t)((target_block.m_height / 2))));
    Serial.print("\n");

    // target object close enough?
    if (
        (target_block.m_y - (uint16_t)((target_block.m_height / 2))) > y_closeness_thr && 
        target_block.m_age > age_thr
    ) {
        Serial.print("Object is close enough\n");
        Serial.print((target_block.m_y - (uint16_t)((target_block.m_height / 2))));
        setMotors(0);
        delay((100 / (5 * PI * wheel_radius)) * 1000);
        analogWrite(motor_l_speed_pin, 0);
        analogWrite(motor_r_speed_pin, 0);
        delay(500);
        bot_state = BotState::COL_GRAB;
    }

    int x_pos = target_block.m_x;

    Serial.print("Error: ");
    Serial.print(x_pos - 157);
    Serial.print("\n");

    // get optimal rotation speed
    target_rotation_speed = controller.updatePD(x_pos - 157);

    Serial.print("\tRequested rotation speed: ");
    Serial.print(target_rotation_speed);
    Serial.print("\n");

    // set motors
    setMotors(target_rotation_speed);
}

void complianceTesting::navigateDep() {
    double delta_scale = (w_cur * h_cur) / (w_prev * h_prev);
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

    target_rotation_speed = controller.updatePD((double)(pixy.ccc.blocks[target_index].m_x - 157));

    setMotors(target_rotation_speed);
}

Block complianceTesting::getDetection(uint16_t index) {
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      if (pixy.ccc.blocks[i].m_index == index) {
          return pixy.ccc.blocks[i];
      }
  }

  Block empty;

  empty.m_age = 0;
  empty.m_angle = 0;
  empty.m_height = 0;
  empty.m_index = 0;
  empty.m_signature = 0;
  empty.m_width = 0;
  empty.m_x = 0;
  empty.m_y = 0;

  return empty;
}

bool complianceTesting::checkBlock(Block block) {
    bool empty_check1 = block.m_age || block.m_angle || block.m_height || block.m_index || block.m_signature || block.m_width;
    bool empty_check2 = block.m_x || block.m_y;
    bool empty_check = empty_check1 || empty_check2;

    if (!empty_check) {
      return false;
    }

    double delta_scale = (w_cur * h_cur) / (w_prev * h_prev);
    delta_scale = abs(1 - delta_scale);

    if (
        delta_scale > bounding_box_thr && 
        abs(block.m_age - age_prev) > age_thr &&
        target_colour != block.m_signature
    ) {
        return false;
    }

    return true;

}
