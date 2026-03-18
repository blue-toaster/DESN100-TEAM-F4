# DESN1000 F04 codebase

This is the code repository for the T1 F04 DESN1000 team

If ur not part of the team please don't copy our code like that's just kinda mean

yo eason u can checkout some of the code and take a look and how stuff works. Ill help u get set up on platformio

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
modesEnum run_mode = modesEnum::MOTORS;
```
