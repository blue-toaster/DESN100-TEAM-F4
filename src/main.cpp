#include "ultrasound_tests/ultrasound_test1.hpp"
#include "motor_tests/motor_test1.hpp"
#include "pixy_cam_calibration/pixy_test.hpp"
#include "compliance_testing/compliance_testing.hpp"
#include "interfaces.hpp"

// Mode enum definition
modesEnum run_mode = modesEnum::COMPLIANCE;

// Run modes definitions
ultrasonicTest ult_test;
motorTest motor_test;
pixyTest pixy_test;
complianceTesting compliance_test;

void setup() {
  // put your setup code here, to run once:
  switch(run_mode) {
    case modesEnum::ULTRASONIC:
      ult_test.init();
      break;
    
    case modesEnum::MOTORS:
      motor_test.init();
      break;

    case modesEnum::PIXYCAM:
      pixy_test.init();
      break;

    case modesEnum::IRSENSOR:
      // to be coded
      break;

    case modesEnum::COMPLIANCE:
      compliance_test.init();
      break;

    case modesEnum::MAIN:
      // to be coded
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(run_mode) {
    case modesEnum::ULTRASONIC:
      ult_test.run();
      break;
    
    case modesEnum::MOTORS:
      motor_test.run();
      break;

    case modesEnum::PIXYCAM:
      pixy_test.run();
      break;

    case modesEnum::IRSENSOR:
      // to be coded
      break;

    case modesEnum::COMPLIANCE:
      compliance_test.run();
      break;

    case modesEnum::MAIN:
      // to be coded
      break;
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
  
}
