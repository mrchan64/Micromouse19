/************************************************************
 *                                                          *
 *   This FILE sets up the sensors used in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "SensorComm.hpp"

// OBJECTS ---------------------------------------------------
I2C i2c(M_I2C_SDA, M_I2C_SCL);
DigitalOut gyro_led(LED_GYRO);
DigitalOut accel_led(LED_ACCEL);

// CLASSES ---------------------------------------------------
Gyro::Gyro(int a){
  this->baseline.x = 0;
  this->baseline.y = 0;
  this->baseline.z = 0;
  this->reset();
}

void Gyro::reset(){
  gyro_led = 1;

  char dat[2];
  dat[0] = GYRO_CHIP_ID;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines chip id register
  i2c.read(GYRO_ADDR, dat, 1);        // 0xD7
  // pc.printf("Val: %hhx %x\n", dat[0], dat[0]);
  if(dat[0]!=GYRO_I2C_ID){
    this->error = 1;
  }
  else {
    this->error = 0;
    gyro_led = 0;
  }

  // Reset then switch to normal mode and enable all three channels
  dat[0] = GYRO_CTRL_1;
  dat[1] = 0x00;
  i2c.write(GYRO_ADDR, dat, 2);
  dat[1] = 0x0F;    // try FF for high freq sensing
  i2c.write(GYRO_ADDR, dat, 2);

  // set resolution to 500dps
  dat[0] = GYRO_CTRL_4;
  dat[1] = 0x10;
  i2c.write(GYRO_ADDR, dat, 2);

}

SensorData Gyro::read(){

  char dat[2];
  dat[0] = GYRO_X_LOW;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines x lo register
  i2c.read(GYRO_ADDR, dat, 1);
  char xlo = dat[0];
  dat[0] = GYRO_X_HIGH;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines x hi register
  i2c.read(GYRO_ADDR, dat, 1);
  char xhi = dat[0];
  dat[0] = GYRO_Y_LOW;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines y lo register
  i2c.read(GYRO_ADDR, dat, 1);
  char ylo = dat[0];
  dat[0] = GYRO_Y_HIGH;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines y hi register
  i2c.read(GYRO_ADDR, dat, 1);
  char yhi = dat[0];
  dat[0] = GYRO_Z_LOW;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines z lo register
  i2c.read(GYRO_ADDR, dat, 1);
  char zlo = dat[0];
  dat[0] = GYRO_Z_HIGH;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines z hi register
  i2c.read(GYRO_ADDR, dat, 1);
  char zhi = dat[0];

  int16_t x_raw = xlo | (xhi << 8);
  int16_t y_raw = ylo | (yhi << 8);
  int16_t z_raw = zlo | (zhi << 8);

  // pc.printf("%d %d %d %d %d %d\n", xlo, xhi, ylo, yhi, zlo, zhi);

  SensorData sd;

  sd.x = (float) (x_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.x;
  sd.y = (float) (y_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.y;
  sd.z = (float) (z_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.z;

  return sd;

}

void Gyro::normalize(){
  // normalization
  SensorData total = {0, 0, 0};
  SensorData gdat;
  for(int i = 0; i<NORMALIZATION_WINDOW; i++){
    gdat = this->read();
    total.x += gdat.x;
    total.y += gdat.y;
    total.z += gdat.z;
  }
  this->baseline.x += total.x / NORMALIZATION_WINDOW;
  this->baseline.y += total.y / NORMALIZATION_WINDOW;
  this->baseline.z += total.z / NORMALIZATION_WINDOW;
}

Accel::Accel(int a){
  this->reset();
}

void Accel::reset(){
  accel_led = 1;

  char dat[2];
  dat[0] = ACCEL_CHIP_ID;
  i2c.write(ACCEL_ADDR, dat, 1, true); // defines chip id register
  i2c.read(ACCEL_ADDR, dat, 1);        // 0x33
  // pc.printf("Val: %hhx %x\n", dat[0], dat[0]);
  if(dat[0]!=ACCEL_I2C_ID){
    this->error = 1;
  }
  else {
    this->error = 0;
    accel_led = 0;
  }

  // Reset then switch to normal mode and enable all three channels
  dat[0] = ACCEL_CTRL_1;
  // dat[1] = 0x00;
  // i2c.write(ACCEL_ADDR, dat, 2);
  dat[1] = 0x6F;  // data rate: 1.620 kHz
  i2c.write(ACCEL_ADDR, dat, 2);

  // set full scale to 8g
  dat[0] = ACCEL_CTRL_4;
  dat[1] = 0x20;
  i2c.write(ACCEL_ADDR, dat, 2);

}

SensorData Accel::read(){

  char dat[2];
  dat[0] = ACCEL_X_H;
  i2c.write(ACCEL_ADDR, dat, 1, true); // defines x h register
  i2c.read(ACCEL_ADDR, dat, 1);
  char x_raw = dat[0];
  dat[0] = ACCEL_Y_H;
  i2c.write(ACCEL_ADDR, dat, 1, true); // defines y h register
  i2c.read(ACCEL_ADDR, dat, 1);
  char y_raw = dat[0];
  dat[0] = ACCEL_Z_H;
  i2c.write(ACCEL_ADDR, dat, 1, true); // defines z h register
  i2c.read(ACCEL_ADDR, dat, 1);
  char z_raw = dat[0];

  SensorData sd;

  sd.x = (float) ((int8_t) x_raw);
  sd.y = (float) ((int8_t) y_raw);
  sd.z = (float) ((int8_t) z_raw);

  return sd;

}

void Accel::normalize(){

}

Gyro m_gyro(1);
Accel m_accel(1);

// METHODS ---------------------------------------------------
void reset_sensors(){
  m_gyro.reset();
  m_accel.reset();
}

SensorData read_gyro(){
  return m_gyro.read();
}

SensorData read_accel(){
  return m_accel.read();
}

uint8_t read_sensor_error_state(){
  return (m_gyro.error << 1) | m_accel.error;
}

void normalize_sensors(){
  m_gyro.normalize();
  m_accel.normalize();
}

bool paused = false;

bool check_pause_state(){
  SensorData a_data = read_accel();
  a_data.x = abs(a_data.x); a_data.y = abs(a_data.y); a_data.z = abs(a_data.z);
  if(!paused && a_data.z < 3 && (a_data.x > 10 || a_data.y > 10)){
    paused = true;
  }else if(paused && a_data.x < 2 && a_data.y < 2 && a_data.z > 13){
    paused = false;
  }
  return paused;
}