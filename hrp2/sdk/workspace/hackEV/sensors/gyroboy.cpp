/**
 * @file    gyroboy.cpp
 * @brief   This file has the gyro controll feature.
 *
 * References:
 * http://www.hitechnic.com/blog/gyro-sensor/htway/
 * http://www.cs.bgu.ac.il/~ami/teaching/Lejos-2013/classes/src/lejos/robotics/navigation/Segoway.java
*/

#include "ev3api.h"
#include "utilities.h"
#include "gyroboy.h"

//! \addtogroup Constants for the self-balance control algorithm.
//@{
const float KSTEER=-0.25;
const float EMAOFFSET = 0.0005f;
const float KGYROANGLE = 7.5f;
const float KGYROSPEED = 1.15f;
const float KPOS = 0.07f;
const float KSPEED = 0.1f;
const float KDRIVE = -0.02f;
const float WHEEL_DIAMETER = 5.6;
const uint32_t WAIT_TIME_MS = 5;
const uint32_t FALL_TIME_MS = 1000;
const float INIT_GYROANGLE = -0.25;
const float INIT_INTERVAL_TIME = 0.014;
//@}

#if 0
/**
 * Constants for the self-balance control algorithm. (Gyroboy Version)
 */
//const float EMAOFFSET = 0.0005f, KGYROANGLE = 15.0f, KGYROSPEED = 0.8f, KPOS = 0.12f, KSPEED = 0.08f, KDRIVE = -0.01f;
//const float WHEEL_DIAMETER = 5.6;
//const uint32_t WAIT_TIME_MS = 1;
//const uint32_t FALL_TIME_MS = 1000;
//const float INIT_GYROANGLE = -0.25;
//const float INIT_INTERVAL_TIME = 0.014;
#endif

//! \addtogroup Global variables used by the self-balance control algorithm.
//@{
static int motor_diff = 0;
static int motor_diff_target = 0;
static int loop_count = 0;
static int motor_control_drive = 0;
static int motor_control_steer = 0;

//! the offset for calibration.
static float gyro_offset = 0;

//! the speed of the gyro sensor after calibration.
static float gyro_speed = 0;

//! the angle of the robot.
static float gyro_angle = 0;

static float interval_time = 0;
static float motor_pos, motor_speed = 0;
//@}

/**
 * @brief   Calculate the initial gyro offset for calibration.
 * @par Refer
 *  - 参照する変数 gyro_offset
 * @return E_OK 範囲内
 * @return E_OBJ 範囲外
*/
static ER calibrate_gyro_sensor() {
    int gMn = 1000, gMx = -100, gSum = 0;
    for (int i = 0; i < 200; ++i) {
        int gyro = ev3_gyro_sensor_get_rate(EV3_SENSOR_GYRO);
        gSum += gyro;
        if (gyro > gMx) {
            gMx = gyro;
        }

        if (gyro < gMn) {
            gMn = gyro;
        }

        tslp_tsk(4);
    }

    if(!(gMx - gMn < 2)) { // TODO: recheck the condition, '!(gMx - gMn < 2)' or '(gMx - gMn < 2)'
        gyro_offset = gSum / 200.0f;
        return E_OK;
    }

    return E_OBJ;
}

/**
 * @brief Calculate the average interval time of the main loop for the self-balance control algorithm by seconds(Units).
 *
 * @par Refer
 *  - 参照する変数 loop_count
 *  - 参照する変数 interval_time
 *  - 参照する定数 INIT_INTERVAL_TIME
 *
 * @return  なし
*/
static void update_interval_time() {
    static SYSTIM start_time;

    if(loop_count++ == 0) {
        // Interval time for the first time (use 6ms as a magic number)
        //interval_time = 0.006;
        interval_time = INIT_INTERVAL_TIME;
        ER ercd = get_tim(&start_time);
        assert(ercd == E_OK);
    } else {
        SYSTIM now;
        ER ercd = get_tim(&now);
        assert(ercd == E_OK);
        interval_time = ((float)(now - start_time)) / loop_count / 1000;
    }
}

/**
 * @brief Update data of the gyro sensor.
 *
 * @par Refer
 *  - 参照する変数 gyro_offset
 *  - 参照する変数 gyro_speed
 *  - 参照する変数 gyro_angle
 *  - 参照する変数 interval_time
 *
 * @return  なし
*/
static void update_gyro_data() {
    int gyro = ev3_gyro_sensor_get_rate(EV3_SENSOR_GYRO);
    gyro_offset = EMAOFFSET * gyro + (1 - EMAOFFSET) * gyro_offset;
    gyro_speed = gyro - gyro_offset;
    gyro_angle += gyro_speed * interval_time;
}

/**
 * @brief Update data of the motors
 *
 * @par Refer
 *  - 参照する変数 motor_pos
 *  - 参照する変数 prev_motor_cnt_sum
 *  - 参照する変数 motor_diff
 *  - 参照する変数 motor_speed
 *
 * @return  なし
*/
static void update_motor_data() {
    static int32_t prev_motor_cnt_sum, motor_cnt_deltas[4];

    if (loop_count == 1) {
        // Reset
        motor_pos = 0;
        prev_motor_cnt_sum = 0;
        motor_cnt_deltas[0] = motor_cnt_deltas[1] = motor_cnt_deltas[2] = motor_cnt_deltas[3] = 0;
    }

    int32_t left_cnt = ev3_motor_get_counts(EV3_MOTOR_LEFT);
    int32_t right_cnt = ev3_motor_get_counts(EV3_MOTOR_RIGHT);
    int32_t motor_cnt_sum = left_cnt + right_cnt;
    motor_diff = right_cnt - left_cnt; // TODO: with diff
    int32_t motor_cnt_delta = motor_cnt_sum - prev_motor_cnt_sum;

    prev_motor_cnt_sum = motor_cnt_sum;
    motor_pos += motor_cnt_delta;
    motor_cnt_deltas[loop_count % 4] = motor_cnt_delta;
    motor_speed = (motor_cnt_deltas[0] + motor_cnt_deltas[1] + motor_cnt_deltas[2] + motor_cnt_deltas[3]) / 4.0f / interval_time;
}

/**
 * @brief Control the power to keep balance.
 *
 * @par Refer
 *  - 参照する変数 loop_count
 *  - 参照する定数 WHEEL_DIAMETER
 *  - 参照する変数 motor_pos
 *  - 参照する変数 motor_control_drive
 *  - 参照する変数 interval_time
 *  - 参照する変数 gyro_speed
 *  - 参照する定数 KGYROSPEED
 *  - 参照する定数 KGYROANGLE
 *  - 参照する定数 KPOS
 *  - 参照する定数 KDRIVE
 *  - 参照する定数 KSPEED
 *  - 参照する変数 gyro_angle
 *  - 参照する変数 motor_speed
 *
 * @return  false   when the robot has fallen.
 * @return  true    succeed
*/
static bool_t keep_balance() {
    static SYSTIM ok_time;

    if (loop_count == 1) {
        // Reset ok_time
        get_tim(&ok_time);
    }

    float ratio_wheel = WHEEL_DIAMETER / 5.6;

    // Apply the drive control value to the motor position to get robot to move.
    motor_pos -= motor_control_drive * interval_time;

    // This is the main balancing equation
    int power = (int)((KGYROSPEED * gyro_speed +               // Deg/Sec from Gyro sensor
                       KGYROANGLE * gyro_angle) / ratio_wheel + // Deg from integral of gyro
                       KPOS       * motor_pos +                // From MotorRotaionCount of both motors
                       KDRIVE     * motor_control_drive +       // To improve start/stop performance
                       KSPEED     * motor_speed);              // Motor speed in Deg/Sec

    // Check fallen
    SYSTIM time;
    get_tim(&time);
    if (power > -100 && power < 100) {
        ok_time = time;
    }
    else if (time - ok_time >= FALL_TIME_MS) {
        return false;
    }

    // Steering control
    motor_diff_target += motor_control_steer * interval_time;

    int left_power, right_power;

    // TODO: support steering and motor_control_drive
    int power_steer = (int)(KSTEER * (motor_diff_target - motor_diff));
    left_power = power + power_steer;
    right_power = power - power_steer;
    if (left_power > 100) {
        left_power = 100;
    }

    if (left_power < -100) {
        left_power = -100;
    }

    if (right_power > 100) {
        right_power = 100;
    }

    if (right_power < -100) {
        right_power = -100;
    }

    ev3_motor_set_power(EV3_MOTOR_LEFT, (int)left_power);
    ev3_motor_set_power(EV3_MOTOR_RIGHT, (int)right_power);

    return true;
}

void balance_task(intptr_t unused) {
    ER ercd;

    /**
     * Reset
     */
    loop_count = 0;
    motor_control_drive = 0;
    ev3_motor_reset_counts(EV3_MOTOR_LEFT);
    ev3_motor_reset_counts(EV3_MOTOR_RIGHT);
    //TODO: reset the gyro sensor
    ev3_gyro_sensor_reset(EV3_SENSOR_GYRO);

    /**
     * Calibrate the gyro sensor and set the led to green if succeeded.
     */
    _debug(syslog(LOG_NOTICE, "Start calibration of the gyro sensor."));
    for(int i = 10; i > 0; --i) { // Max retries: 10 times.
        ercd = calibrate_gyro_sensor();
        if(ercd == E_OK) break;
        if (i != 1) {
            syslog(LOG_ERROR, "Calibration failed, retry.");
        }
        else {
            syslog(LOG_ERROR, "Max retries for calibration exceeded, exit.");
            return;
        }
    }
    _debug(syslog(LOG_INFO, "Calibration succeed, offset is %de-3.", (int)(gyro_offset * 1000)));
    gyro_angle = INIT_GYROANGLE;
    ev3_led_set_color(LED_GREEN);

    /**
     * Main loop for the self-balance control algorithm
     */
    while(1) {
        // Update the interval time
        update_interval_time();

        // Update data of the gyro sensor
        update_gyro_data();

        // Update data of the motors
        update_motor_data();

        // Keep balance
        if(!keep_balance()) {
            ev3_motor_stop(EV3_MOTOR_LEFT, false);
            ev3_motor_stop(EV3_MOTOR_RIGHT, false);
            ev3_led_set_color(LED_RED); // TODO: knock out
            syslog(LOG_NOTICE, "Knock out!");
            return;
        }

        tslp_tsk(WAIT_TIME_MS);
    }
}

static FILE *bt = NULL;

void idle_task(intptr_t unused) {
    while(1) {
    	fprintf(bt, "Press 'h' for usage instructions.\n");
    	tslp_tsk(1000);
    }
}

void driveController(){
    while (!ev3_bluetooth_is_connected()) tslp_tsk(100);
    uint8_t c = fgetc(bt);
    sus_tsk(IDLE_TASK);
    
    switch(c) {
    case 'w':
        if(motor_control_drive < 0)
            motor_control_drive = 0;
        else
            motor_control_drive += 10;
        fprintf(bt, "motor_control_drive: %d\n", motor_control_drive);
        break;

    case 's':
        if(motor_control_drive > 0)
            motor_control_drive = 0;
        else
            motor_control_drive -= 10;
        fprintf(bt, "motor_control_drive: %d\n", motor_control_drive);
        break;

    case 'a':
        if(motor_control_steer < 0)
            motor_control_steer = 0;
        else
            motor_control_steer += 10;
        fprintf(bt, "motor_control_steer: %d\n", motor_control_steer);
        break;

    case 'd':
        if(motor_control_steer > 0)
            motor_control_steer = 0;
        else
            motor_control_steer -= 10;
        fprintf(bt, "motor_control_steer: %d\n", motor_control_steer);
        break;

    case 'h':
        fprintf(bt, "==========================\n");
        fprintf(bt, "Usage:\n");
        fprintf(bt, "Press 'w' to speed up\n");
        fprintf(bt, "Press 's' to speed down\n");
        fprintf(bt, "Press 'a' to turn left\n");
        fprintf(bt, "Press 'd' to turn right\n");
        fprintf(bt, "Press 'i' for idle task\n");
        fprintf(bt, "Press 'h' for this message\n");
        fprintf(bt, "==========================\n");
        break;

    case 'i':
        fprintf(bt, "Idle task started.\n");
        rsm_tsk(IDLE_TASK);
        break;

    default:
        fprintf(bt, "Unknown key '%c' pressed.\n", c);
    }
}

void drive_controller_task(intptr_t unused){
    while(1){
        driveController();
    }
}

void initialize_gyroboy(intptr_t unused) {
    // Draw information
    lcdfont_t font = EV3_FONT_MEDIUM;
    ev3_lcd_set_font(font);
    int32_t fontw, fonth;
    ev3_font_get_size(font, &fontw, &fonth);
    char lcdstr[100];
    ev3_lcd_draw_string("App: Gyroboy", 0, 0);
    sprintf(lcdstr, "Port%c:Gyro sensor", '1' + EV3_SENSOR_GYRO);
    ev3_lcd_draw_string(lcdstr, 0, fonth);
    sprintf(lcdstr, "Port%c:Left motor", 'A' + EV3_MOTOR_LEFT);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 2);
    sprintf(lcdstr, "Port%c:Right motor", 'A' + EV3_MOTOR_RIGHT);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 3);

    // Start task for self-balancing
    act_tsk(BALANCE_TASK);

    // Open Bluetooth file
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    // Start task for printing message while idle
	act_tsk(IDLE_TASK);
    
    act_tsk(DRIVE_CONTROLLER_TASK);
    
}
