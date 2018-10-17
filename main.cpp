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

union float4serial {
    float fl;
    char buff[sizeof(float)];
};

Motor m_left('l');
Motor m_right('r');

void pre_code() {
    pc.printf("\n");
    wait(.5);
    cycle_leds(.25f);
    initialize_IR();
    normalize_sensors();
    // wait(.5); // WHY DOES THIS WAIT CAUSE IR INPUT TO BE FUCKED UP ???????
    uint8_t s_e_s = read_sensor_error_state();
    if (s_e_s) pc.printf("Sensor error: %d\n", s_e_s);
    wait(.1);
    pc.printf("Voltage: %f\n", voltmeter); // ?????
    cycle_leds(.25f);
}

int test_method(){
    IRData dat;
    IRData dat2;
    while(true){
        run_IR_cycle();
        dat = get_current_data();
        dat2 = get_curr_norm_data();
        pc.printf("D %f %f %f %f %f %f %f %f\n", dat.lf, dat.l, dat.r, dat.rf, dat2.lf, dat2.l, dat2.r, dat2.rf);
        // pc.printf("D %.4f %.4f %.4f %.4f\n", dat.lf, dat.l, dat.r, dat.rf);

        // aligning to wall
        int stepl = dat2.l>0 ? ceil(dat2.l/.005) : floor(dat2.l/.005);
        int stepr = dat2.r>0 ? ceil(dat2.r/.005) : floor(dat2.r/.005);
        m_left.setStepAboveSpeed(0.0f, -stepl);
        m_right.setStepAboveSpeed(0.0f, -stepr);
    }
}

int main() {
    pre_code();
    wait(1);
    // test_method();
    SensorData g_dat = read_gyro();
    SensorData a_dat = read_accel();
    pc.printf("Gyro: %f %f %f Accel %f %f %f\n", g_dat.x, g_dat.y, g_dat.z, a_dat.x, a_dat.y, a_dat.z);

    wait(1);
    float n_deg = .9f;
    int ticks = .25f * TURN_DIAM / WHEEL_DIAM * ONE_REV;
    int oticks = TURN_DIAM / WHEEL_DIAM * ONE_REV;

    // while(m_left.getTicks() < 100){
    //     m_left.setSpeed(.091);
    //     m_right.setSpeed(.091);
    // }

    // while(m_left.getTicks() < ONE_REV-100){
    //     m_left.setSpeed(.15);
    //     m_right.setSpeed(.15);
    // }

    int c1 = 0;
    while(m_right.getTicks() < ticks){
        // m_left.setSpeed(.12);
        m_right.setSpeed(.12);
        // pc.printf("%d %d\n",ticks, m_right.getTicks());
        c1++;
    }
    m_left.setSpeed(0);
    m_right.setSpeed(0);
    wait(1);

    int c2 = 0;
    while(m_left.getTicks() < ticks){
        m_left.setSpeed(.12);
        // m_right.setSpeed(.12);
        // pc.printf("%d %d\n",ticks, m_right.getTicks());
        c2++;
    }

    m_left.setSpeed(0);
    m_right.setSpeed(0);
    pc.printf("Tar: %d Act: %d %d\n", ticks, m_left.getTicks(), m_right.getTicks());
    pc.getc();
    pc.printf("%d %d ugh\n",c1,c2);

}