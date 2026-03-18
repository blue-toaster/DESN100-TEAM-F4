#include <Arduino.h>
#include <Pixy2.h>

#pragma once

enum class modesEnum {
    MOTORS,
    ULTRASONIC,
    PIXYCAM,
    IRSENSOR,
    COMPLIANCE,
    MAIN
};

class modesObject 
{
public:
    virtual void run() = 0;

    virtual void init() = 0;
};
