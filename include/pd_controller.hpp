#ifndef PD_CONTROLLER_H
#define PD_CONTROLLER_H

#include "interfaces.hpp"

class PDController 
{
public:
    void setCoefficients(double p, double d);

    double updatePD(double err);

private:
    // Control coefficients
    double K_p = 0;
    double K_d = 0;

    // Error Variables
    double e_prev = 0;

    // Constants
    const double dt = (1.0 / 60.0);
    const double axle_length = 16.5;
    const double wheel_radius = 3.5;
    const int max_RPM = 150;

};


#endif
