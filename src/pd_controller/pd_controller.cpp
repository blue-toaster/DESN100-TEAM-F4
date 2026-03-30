#include "pd_controller.hpp"

void PDController::setCoefficients(double p, double d) {
    K_d = d;
    K_p = p;
}

double PDController::updatePD(double pos) {

    double sp = 157;

    double err = pos - sp;



    double output = ((K_p * err) + (K_d * (-1 * (pos - e_prev) / dt))) / 50;

    e_prev = pos;

    Serial.print("\tCalculated Rotation: ");
    Serial.print(output);
    Serial.print("\n");

    if (abs(output) > (max_RPM * PI * wheel_radius / (15 * axle_length))) {
        output = (output / abs(output)) * (10 * PI * wheel_radius / axle_length);
    }

    return output;
}
