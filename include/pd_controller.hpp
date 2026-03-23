#ifndef PD_CONTROLLER_H
#define PD_CONTROLLER_H

class PDController 
{
public:
    void setCoefficients(long p, long d);

    long updatePD(long err);

private:
    // Control coefficients
    long K_p = 0;
    long K_d = 0;

    // Error Variables
    long e_prev = 0;

    // Constants
    const long dt = (1 / 60);
    const long axle_length = 1;

};


#endif
