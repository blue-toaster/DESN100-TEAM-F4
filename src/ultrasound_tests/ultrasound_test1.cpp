#include "ultrasound_tests/ultrasound_test1.hpp"

void ultrasonicTest::init() {
    pinMode(ultrasonic_trigger_pin, OUTPUT);
    pinMode(ultrasonic_echo_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
}

void ultrasonicTest::run() {
    long distance = readUltrasonic(ultrasonic_trigger_pin, ultrasonic_echo_pin);

    if (distance > 30 && distance < 200) {
        long freq = map(distance, ultrasonic_range[0], ultrasonic_range[1], buzzer_range[0], buzzer_range[1]);
        tone(buzzer_pin, (int)freq);
    } else {
        noTone(buzzer_pin);
    }
}

long ultrasonicTest::readUltrasonic(const int trig, const int echo) {
    // set trig 0
    digitalWrite(trig, LOW);

    // send pulse
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // return distance
    return (ultrasonic_multi_factor * pulseIn(echo, HIGH));
}
