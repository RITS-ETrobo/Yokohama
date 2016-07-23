/**
 * @file    utilities.cpp
 * @brief   This file has the utilitiy functions.
 */

#include "ev3api.h"
#include "utilities.h"

/**
 * @brief   LCDに数値を表示させる
 * 
 * @param value 表示させる値
 * @return  なし
*/
void writeFloatLCD(float value){
    char message[16];
    memset(message, '\0', sizeof(message));
    sprintf(message, "%03.03f", value); 
    OUTPUT_LOG(message, OUTPUT_TYPE_LCD);
}

/**
 * @brief   モーターの設定を行う
 * @return  なし
*/
void configure_motors()
{
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_motor_config(arm_motor, LARGE_MOTOR);//MEDIUMとどちらがいいか要検証
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
