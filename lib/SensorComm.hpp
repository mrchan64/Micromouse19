/************************************************************
 *                                                          *
 *   This FILE sets up the sensors used in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan

#ifndef SENSORCOMM_H
#define SENSORCOMM_H

#include "mbed.h"
#include "PinOut.hpp"

// I2C ADDRESSES ---------------------------------------------
const char GYRO_ADDR      = 0xD6;
const char ACCEL_ADDR     = 0x32;
const char GYRO_I2C_ID    = 0xD7;
const char ACCEL_I2C_ID   = 0x33;
const int I2C_FREQ        = 100000;

// I2C REGISTERS ---------------------------------------------
const char GYRO_CHIP_ID   = 0x0F;
const char GYRO_CTRL_1    = 0X20;
const char GYRO_CTRL_4    = 0X23;
const char GYRO_X_LOW     = 0x28;
const char GYRO_X_HIGH    = 0x29;
const char GYRO_Y_LOW     = 0x2A;
const char GYRO_Y_HIGH    = 0x2B;
const char GYRO_Z_LOW     = 0x2C;
const char GYRO_Z_HIGH    = 0x2D;

const char ACCEL_CHIP_ID  = 0x0F;
const char ACCEL_CTRL_1   = 0x20;
const char ACCEL_CTRL_4   = 0x23;
const char ACCEL_X_H      = 0x29;
const char ACCEL_Y_H      = 0x2B;
const char ACCEL_Z_H      = 0x2D;

// GYRO CONSTANTS --------------------------------------------
const float GYRO_SENSITIVITY_500DPS = 0.0175F;
const int NORMALIZATION_DELAY_US    = 100;
const int NORMALIZATION_WINDOW      = 100;

// OBJECTS ---------------------------------------------------
extern I2C i2c;

// DATA STRUCTS ----------------------------------------------
struct SensorData {
  float x;
  float y;
  float z;
};

// enum AsyncPollState {
//   ASYNC_IDLE,
//   ASYNC_START,
//   READ_GYRO_Z_LO,
//   READ_GYRO_Z_HI,
//   READ_ACCEL_X,
//   READ_ACCEL_Y,
//   READ_ACCEL_Z
// };

// CLASSES ---------------------------------------------------
class Gyro {
public:
  Gyro(int a);
  void reset();
  SensorData read();
  void normalize();
  uint8_t error;
  SensorData baseline;
};

class Accel {
public:
  Accel(int a);
  void reset();
  SensorData read();
  void normalize();
  uint8_t error;
  SensorData baseline;
};

// MORE OBJECTS ----------------------------------------------
// extern Gyro m_gyro;
// extern Accel m_accel;

// METHODS ---------------------------------------------------
void reset_sensors();
// SensorData read_gyro();
// SensorData read_accel();
uint8_t read_sensor_error_state();
void normalize_sensors();
bool check_pause_state();
// void run_async_sensor_cycle();
// void on_async_sensor_cycle(bool run);
void run_sensor_cycle();
SensorData get_gyro_data();
SensorData get_accel_data();
void reset_angle_tracker();
float get_curr_angle();

#endif