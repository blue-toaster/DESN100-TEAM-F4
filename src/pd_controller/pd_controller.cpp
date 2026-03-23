#include "pd_controller.hpp"

void PDController::setCoefficients(long p, long d) {
    K_d = d;
    K_p = p;
}

long PDController::updatePD(long err) {
    long output = (K_p * err) + (K_d * e_prev);

    e_prev = err;

    if (output > (300/axle_length)) {
        output = 300 / axle_length;
    }

    return output;
}
