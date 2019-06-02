/************************************************************
 *                                                          *
 *   This FILE sets up the sensors used in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "SensorComm.hpp"
#include "Serial.hpp"

// OBJECTS ---------------------------------------------------
I2C i2c(M_I2C_SDA, M_I2C_SCL);
SensorData gyro_dat = {0.0f, 0.0f, 0.0f};
SensorData accel_dat = {0.0f, 0.0f, 0.0f};
// AsyncPollState currPollState = ASYNC_IDLE;
// char i2c_dat[1];
// char t_zlo;
// SensorData temp_gyro = {0.0f, 0.0f, 0.0f};
// SensorData temp_accel = {0.0f, 0.0f, 0.0f};
// bool async_g_z_rdy = false;
// bool async_g_z_lo_rdy = false;
// bool async_a_x_rdy = false;
// bool async_a_y_rdy = false;
// bool async_a_z_rdy = false;
// bool async_g_rdy = false;
// bool async_a_rdy = false;
// Thread sens_async_thread;

// angle integration
float curr_angle = 0.0f;
Timer t2;
float lastTime2 = 0.0f;
float timeDiff2 = 0.0f;
float lastval2 = 0.0f;

// CLASSES ---------------------------------------------------
Gyro::Gyro(int a){
  this->baseline.x = 0;
  this->baseline.y = 0;
  this->baseline.z = 0;
  this->reset();
}

void Gyro::reset(){

  i2c.frequency(I2C_FREQ);
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

// SensorData Gyro::read(){

//   char dat[2];
//   dat[0] = GYRO_X_LOW;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines x lo register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char xlo = dat[0];
//   dat[0] = GYRO_X_HIGH;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines x hi register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char xhi = dat[0];
//   dat[0] = GYRO_Y_LOW;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines y lo register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char ylo = dat[0];
//   dat[0] = GYRO_Y_HIGH;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines y hi register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char yhi = dat[0];
//   dat[0] = GYRO_Z_LOW;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines z lo register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char zlo = dat[0];
//   dat[0] = GYRO_Z_HIGH;
//   i2c.write(GYRO_ADDR, dat, 1, true); // defines z hi register
//   i2c.read(GYRO_ADDR, dat, 1);
//   char zhi = dat[0];

//   int16_t x_raw = xlo | (xhi << 8);
//   int16_t y_raw = ylo | (yhi << 8);
//   int16_t z_raw = zlo | (zhi << 8);

//   // pc.printf("%d %d %d %d %d %d\n", xlo, xhi, ylo, yhi, zlo, zhi);

//   SensorData sd;

//   sd.x = (float) (x_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.x;
//   sd.y = (float) (y_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.y;
//   sd.z = (float) (z_raw * GYRO_SENSITIVITY_500DPS) - this->baseline.z;

//   return sd;

// }

SensorData Gyro::read(){

  char dat[1];
  dat[0] = GYRO_Z_LOW;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines z lo register
  i2c.read(GYRO_ADDR, dat, 1);
  char zlo = dat[0];
  dat[0] = GYRO_Z_HIGH;
  i2c.write(GYRO_ADDR, dat, 1, true); // defines z hi register
  i2c.read(GYRO_ADDR, dat, 1);
  char zhi = dat[0];
  int16_t z_raw = zlo | (zhi << 8);

  // pc.printf("%d %d %d %d %d %d\n", xlo, xhi, ylo, yhi, zlo, zhi);

  SensorData sd;

  sd.x = 0.0f;
  sd.y = 0.0f;
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

  i2c.frequency(I2C_FREQ);
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

// SensorData Accel::read(){

//   char dat[2];
//   dat[0] = ACCEL_X_H;
//   i2c.write(ACCEL_ADDR, dat, 1, true); // defines x h register
//   i2c.read(ACCEL_ADDR, dat, 1);
//   char x_raw = dat[0];
//   dat[0] = ACCEL_Y_H;
//   i2c.write(ACCEL_ADDR, dat, 1, true); // defines y h register
//   i2c.read(ACCEL_ADDR, dat, 1);
//   char y_raw = dat[0];
//   dat[0] = ACCEL_Z_H;
//   i2c.write(ACCEL_ADDR, dat, 1, true); // defines z h register
//   i2c.read(ACCEL_ADDR, dat, 1);
//   char z_raw = dat[0];

//   SensorData sd;

//   sd.x = (float) ((int8_t) x_raw);
//   sd.y = (float) ((int8_t) y_raw);
//   sd.z = (float) ((int8_t) z_raw);

//   return sd;

// }

SensorData Accel::read(){

  char dat[1];
  dat[0] = ACCEL_Z_H;
  i2c.write(ACCEL_ADDR, dat, 1, true); // defines z h register
  i2c.read(ACCEL_ADDR, dat, 1);
  char z_raw = dat[0];

  SensorData sd;

  sd.x = 0;
  sd.y = 0;
  sd.z = (float) ((int8_t) z_raw);

  return sd;

}

void Accel::normalize(){

}

Gyro m_gyro(1);
Accel m_accel(1);

// async resolution stuff
// void resolve_g_z_lo(int error){
//   t_zlo = i2c_dat[0];
//   async_g_z_lo_rdy = true;
// }

// void resolve_g_z_hi(int error){
//   char zhi = i2c_dat[0];
//   int16_t z_raw = t_zlo | (zhi << 8);  
//   tempdat = z_raw;

//   temp_gyro.z = (float) (z_raw * GYRO_SENSITIVITY_500DPS) - m_gyro.baseline.z;
//   async_g_z_rdy = true;
// }

// void resolve_a_x(int error){
//   char x_raw = i2c_dat[0];
//   temp_accel.x = (float) ((int8_t) x_raw);
//   async_a_x_rdy = true;
// }

// void resolve_a_y(int error){
//   char y_raw = i2c_dat[0];
//   temp_accel.y = (float) ((int8_t) y_raw);
//   async_a_y_rdy = true;
// }

// void resolve_a_z(int error){
//   char z_raw = i2c_dat[0];
//   temp_accel.z = (float) ((int8_t) z_raw);
//   async_a_z_rdy = true;
// }

// METHODS ---------------------------------------------------
void reset_sensors(){
  m_gyro.reset();
  m_accel.reset();
}

// deprecate this because moving to async i2c
// SensorData read_gyro(){
//   return m_gyro.read();
// }

// SensorData read_accel(){
//   return m_accel.read();
// }

uint8_t read_sensor_error_state(){
  return (m_gyro.error << 1) | m_accel.error;
}

void normalize_sensors(){
  m_gyro.normalize();
  m_accel.normalize();
}

bool paused = false;

bool check_pause_state(){
  SensorData a_data = accel_dat;
  a_data.x = abs(a_data.x); a_data.y = abs(a_data.y); a_data.z = abs(a_data.z);
  if(!paused && a_data.z < 3/* && (a_data.x > 10 || a_data.y > 10)*/){
    paused = true;
  }else if(paused && /*a_data.x < 2 && a_data.y < 2 &&*/ a_data.z > 13){
    paused = false;
  }
  return paused;
}

// void run_async_sensor_cycle(){
//   switch(currPollState){
//     case ASYNC_IDLE:
//       break;
//     case ASYNC_START: // start here
//       async_g_z_lo_rdy = false;
//       async_g_z_rdy = false;
//       async_a_x_rdy = false;
//       async_a_y_rdy = false;
//       async_a_z_rdy = true;
//       currPollState = READ_GYRO_Z_LO;
//       break;
//     case READ_GYRO_Z_LO:
//       if(async_a_z_rdy){
//         accel_dat.z = temp_accel.z;
//         async_a_z_rdy = false;
//         char dat[1];
//         dat[0] = GYRO_Z_LOW;
//         i2c.transfer(GYRO_ADDR, dat, 1, i2c_dat, 1, resolve_g_z_lo, I2C_EVENT_ALL); 
//         currPollState = READ_GYRO_Z_HI;
//       }
//       break;
//     case READ_GYRO_Z_HI:
//       if(async_g_z_lo_rdy){
//         async_g_z_lo_rdy = false;
//         char dat[1];
//         dat[0] = GYRO_Z_HIGH;
//         i2c_dat[0] = 0x12;
//         i2c.transfer(GYRO_ADDR, dat, 1, i2c_dat, 1, resolve_g_z_hi, I2C_EVENT_ALL); 
//         currPollState = READ_ACCEL_X;
//       }
//       break;
//     case READ_ACCEL_X:
//       if(async_g_z_rdy){
//         gyro_dat.z = temp_gyro.z;
//         async_g_z_rdy = false;

//         // update angle
//         float currTime = t2.read_us() / 1000000.0f;
//         timeDiff2 = currTime - lastTime2;
//         lastTime2 = currTime;
//         curr_angle += gyro_dat.z * timeDiff2;
//         lastval2 = gyro_dat.z;

//         char dat[1];
//         dat[0] = ACCEL_X_H;
//         i2c.transfer(GYRO_ADDR, dat, 1, i2c_dat, 1, resolve_a_x, I2C_EVENT_ALL); 
//         currPollState = READ_ACCEL_Y;
//       }
//       break;
//     case READ_ACCEL_Y:
//       if(async_a_x_rdy){
//         accel_dat.x = temp_accel.x;
//         async_a_x_rdy = false;
//         char dat[1];
//         dat[0] = ACCEL_Y_H;
//         i2c.transfer(GYRO_ADDR, dat, 1, i2c_dat, 1, resolve_a_y, I2C_EVENT_ALL); 
//         currPollState = READ_ACCEL_Z;
//       }
//       break;
//     case READ_ACCEL_Z:
//       if(async_a_y_rdy){
//         accel_dat.y = temp_accel.y;
//         async_a_y_rdy = false;
//         char dat[1];
//         dat[0] = ACCEL_Z_H;
//         i2c.transfer(GYRO_ADDR, dat, 1, i2c_dat, 1, resolve_a_z, I2C_EVENT_ALL); 
//         currPollState = READ_GYRO_Z_LO;
//       }
//       break;

//   }
//       // pc.printf("%.3f\n\r", gyro_dat.z);
// }

// void run_async_sensor_cycle(){
//   if(async_g_rdy){
//     gyro_dat.z = temp_gyro.z;
    
//     float currTime = t2.read_us() / 1000000.0f;
//     timeDiff2 = currTime - lastTime2;
//     lastTime2 = currTime;

//     curr_angle += gyro_dat.z * timeDiff2;

//     async_g_rdy = false;
//   }
//   if(async_a_rdy){
//     accel_dat.x = temp_accel.x;
//     accel_dat.y = temp_accel.y;
//     accel_dat.z = temp_accel.z;
//     async_a_rdy = false;
//   }
// }

// this method is multithreaded cuz i2c transfer is stupid
// void synch_run_sensors(){
//   while(true){
//     if(!async_g_rdy){
//       temp_gyro = read_gyro();
//       async_g_rdy = true;
//     }
//     if(!async_a_rdy){
//       SensorData_adat = read_accel();
//       async_a_rdy = true;
//     }
//   }
// }

// void on_async_sensor_cycle(bool run){
//   // theres no handler for already existing callbacks yet
//   // sens_async_thread.start(synch_run_sensors);
//   if(run){
//     currPollState = ASYNC_START;
//     lastTime2 = 0.0f;
//     timeDiff2 = 0.0f;
//     t2.reset();
//     t2.start();
//   }else{
//     currPollState = ASYNC_IDLE;
//   }
// }

SensorData get_gyro_data(){
  SensorData r_dat;
  r_dat.x = gyro_dat.x;
  r_dat.y = gyro_dat.y;
  r_dat.z = gyro_dat.z;
  return r_dat;
  // return m_gyro.read();
}

SensorData get_accel_data(){
  SensorData r_dat;
  r_dat.x = accel_dat.x;
  r_dat.y = accel_dat.y;
  r_dat.z = accel_dat.z;
  return r_dat;
}

void run_sensor_cycle(){
  // gyro_dat = m_gyro.read();
  // accel_dat = m_accel.read();
  // angle stuff
  // float currTime = t2.read_us() / 1000000.0f;
  // timeDiff2 = currTime - lastTime2;
  // lastTime2 = currTime;
  // curr_angle += gyro_dat.z * timeDiff2;
}

void reset_angle_tracker(){
  curr_angle = 0.0f;
  t2.reset();
  t2.start();
  timeDiff2 = 0;
  lastTime2 = 0;
}

float get_curr_angle(){
  return curr_angle;
  // return curr_angle;
}