/***************************************************
 *                                                 *
 *   This FILE runs the main loop for Micromouse   *
 *                                                 *
 ***************************************************/

// AUTHOR: Matthew Chan

#include "mbed.h"
#include "lib/MiscIO.hpp"
#include "lib/Motors.hpp"
#include "lib/Serial.hpp"
#include "lib/SensorComm.hpp"
#include "lib/Infrared.hpp"
#include "algo/IR_Detect.hpp"
#include "algo/Calibration.hpp"
#include "debug/Debug.hpp"
#include "algo/MazeNode.hpp"

int main() {
    // run_calibration();
    pre_code();
    test_align_front();
}