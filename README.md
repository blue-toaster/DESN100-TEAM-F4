# DESN1000 F04 codebase

This is the code repository for the T1 F04 DESN1000 team

If ur not part of the team please don't copy our code like that's just kinda mean

yo eason u can checkout some of the code and take a look and how stuff works. Ill help u get set up on platformio

# IMPORTANT

ALL UNITS ARE IN mm!!!!

## How to upload to arduino

Install vscode if not already installed

Install vscode PlatformIO extention in vscode

Connect via usb arduino then restart vscode

Click tick or use Ctrl+Alt+B to build - (For non software team this step is not necessary but good to do to check if code works)

Click upload under/next to the build option or use Ctrl+Alt+U to upload the code to the arduino - (Ensure arduino is on)

The arduino should now be running the code

## Project operation

The code runs off of a few different modes 
- ULTRASONIC
- MOTORS
- PIXYCAM
- IRSENSOR
- COMPLIANCE
- MAIN

Before uploading code change 'run_mode' to the desired operation mode (like below):

```cpp
// Variable to define operation mode
modesEnum run_mode = modesEnum::MOTORS;
//                                 ^
//                                 |
//                        (change this guy above)
```

Each mode inherents both run and init from the modesObject class, since all arduino code runs on void setup() and void loop().

In main void setup() is used to run the init function for the selected mode then void loop() is used to constantly run the run() function for the selected mode.

The source code for each mode is placed in folders as shown below:
- ULTRASONIC -> ultrasound_tests
- MOTORS -> motor_drive_tests
- PIXYCAM -> pixy_cam_calibration
- IRSENSOR -> (undefined as of 18/03/2026)
- COMPLIANCE -> compliance_testing
- MAIN -> mike_waste_outski

Their respective header files for object definition are placed within the include directory
