#include <Arduino.h>

// pin definitons
const int ultrasonic_trigger_pin = 0;
const int ultrasonic_echo_pin = 0;
const int buzzer_pin = 0;

// constants
const float ultrasonic_multi_factor = 0.01723;
const int distance_thr = 20;
const int ultrasonic_range[2] = {2, 400};
const int buzzer_range[2] = {1, 5};

// function prototyping
long readUltrasonic(const int trig, const int echo);

void setup() {
    pinMode(ultrasonic_trigger_pin, OUTPUT);
    pinMode(ultrasonic_echo_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
}

void loop() {

    long distance = readUltrasonic(ultrasonic_trigger_pin, ultrasonic_echo_pin);

    if (distance > 30 && distance < 200) {
        long freq = map(distance, ultrasonic_range[0], ultrasonic_range[1], buzzer_range[0], buzzer_range[1]);
        tone(buzzer_pin, (int)freq);
    } else {
        noTone(buzzer_pin);
    }
}

long readUltrasonic(const int trig, const int echo) {
    // set trig 0
    digitalWrite(trig, LOW);

    // send pulse
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // return distance
    return (ultrasonic_multi_factor * pulseIn(echo, HIGH));
}
