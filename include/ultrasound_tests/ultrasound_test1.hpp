#include "interfaces.hpp"

class ultrasonicTest : modesObject 
{
public:
    void run();
    void init();

    long readUltrasonic(const int trig, const int echo);

private:
    // pin definitons
    const int ultrasonic_trigger_pin = 0;
    const int ultrasonic_echo_pin = 0;
    const int buzzer_pin = 0;

    // constants
    const float ultrasonic_multi_factor = 0.01723;
    const int distance_thr = 20;
    const int ultrasonic_range[2] = {2, 400};
    const int buzzer_range[2] = {1, 5};

};
