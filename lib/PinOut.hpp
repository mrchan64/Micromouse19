/************************************************************
 *                                                          *
 *   This FILE defines the pinouts used in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan

#ifndef PINOUT_H
#define PINOUT_H

#include "mbed.h"

// SERIAL ----------------------------------------------------
const PinName M_SERIAL_TX       = PC_10;
const PinName M_SERIAL_RX       = PC_11;

// MOTORS ----------------------------------------------------
const PinName MOTOR_LEFT_A      = PA_2;     // PeripheralPins.c needs to be overridden with the
const PinName MOTOR_LEFT_B      = PA_3;     // uncommented pins for this configuration to work.
const PinName MOTOR_RIGHT_A     = PB_7;
const PinName MOTOR_RIGHT_B     = PB_6;

const PinName ENCODER_LEFT_A    = PA_5;     // Probably need to reverse this (because forward is neg rn)
const PinName ENCODER_LEFT_B    = PA_4;     // (**FIXED**)
const PinName ENCODER_RIGHT_A   = PC_8;
const PinName ENCODER_RIGHT_B   = PC_9;

// INFRARED --------------------------------------------------
const PinName IR_REC_LF         = PC_2;
const PinName IR_REC_L          = PC_3;
const PinName IR_REC_R          = PC_0;
const PinName IR_REC_RF         = PC_1;

const PinName IR_EMIT_LF        = PC_4;
const PinName IR_EMIT_L         = PC_5;
const PinName IR_EMIT_R         = PA_10;
const PinName IR_EMIT_RF        = PA_11;

// SENSORS ---------------------------------------------------
const PinName M_I2C_SDA         = PB_9;
const PinName M_I2C_SCL         = PB_8;

// LEDS ------------------------------------------------------
const PinName LED_1             = PB_12;
const PinName LED_2             = PB_13;
const PinName LED_3             = PB_14;
const PinName LED_4             = PB_15;
const PinName LED_5             = PC_6;

// MISC ------------------------------------------------------
const PinName VOLTMETER         = PA_7;

// -----------------------------------------------------------

#endif