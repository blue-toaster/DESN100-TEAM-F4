#include "pixy_cam_calibration/pixy_test.hpp"

void pixyTest::init() {
    Serial.begin(9000);
    Serial.print("Printing Pixy2 Packets");

    pixy.init();
}

void pixyTest::run() {

    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) {
        Serial.print("Detected ");
        Serial.print(pixy.ccc.numBlocks);
        for (int i = 0; i < pixy.ccc.numBlocks; i++) {
            Serial.print("  block ");
            Serial.print(i);
            Serial.print(": ");
            pixy.ccc.blocks[i].print();
        }
    }
}
