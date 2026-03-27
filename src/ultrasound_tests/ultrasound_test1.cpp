#include "ultrasound_tests/ultrasound_test1.hpp"

void ultrasonicTest::init() {
    pinMode(ultrasonic_trigger_pin, OUTPUT);
    pinMode(ultrasonic_echo_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
    Serial.begin(9000);
}

void ultrasonicTest::run() {
    long distance = readUltrasonic(ultrasonic_trigger_pin, ultrasonic_echo_pin);

    Serial.print("Detected: ");
    Serial.print(distance);
    Serial.print("\n");
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
