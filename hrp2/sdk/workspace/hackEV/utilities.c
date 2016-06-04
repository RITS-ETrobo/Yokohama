/**
 * @file    utilities.c
 * @brief   This file has the utilitiy functions.
 */

#include "ev3api.h"
#include "utilities.h"

//! \addtogroup ポートの設定
//@{
//! Touch sensor    :   Port 1
const int touch_sensor = EV3_PORT_1;

//! Color sensor    :   Port 2
const int color_sensor = EV3_PORT_2;

//! Gyro sensor     :   Port 4
const int gyro_sensor = EV3_PORT_4;

//! Left motor  :   Port C
const int left_motor = EV3_PORT_C;

//! Right motor :   Port B
const int right_motor = EV3_PORT_B;
//@}

/**
 * @brief   モーターの設定を行う
 * @return  なし
*/
void configure_motors()
{
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
}

/**
 * @brief   センサーの設定を行う
 * @return  なし
*/
void configure_sensors()
{
    ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);
    ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
}

/**
 * @brief   光強度の為の測定をおこなう
 * @return  測定された光量値
*/
int calibrate_light_intensity()
{
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_C4, 100);
    // TODO: Calibrate using maximum mode => 100
    // TODO: Calibrate using minimum mode => 0
    printf("Press the touch sensor to measure light intensity.\n");
    while(!ev3_touch_sensor_is_pressed(touch_sensor));
    while(ev3_touch_sensor_is_pressed(touch_sensor));
    int color = ev3_color_sensor_get_reflect(color_sensor);
    return  color;
}
