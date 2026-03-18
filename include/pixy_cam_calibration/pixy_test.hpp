#ifndef PIXY_TEST_H
#define PIXY_TEST_H

#include "interfaces.hpp"

class pixyTest : modesObject 
{
public:
    void run();
    void init();
    
private:
    Pixy2 pixy;
};

#endif
