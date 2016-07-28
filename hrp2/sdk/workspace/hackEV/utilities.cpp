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
    writeStringLCD(message);
}

/**
 * @brief   モーターの設定を行う
 * @return  なし
*/
void configure_motors()
{
    ev3_motor_config(EV3_MOTOR_LEFT, LARGE_MOTOR);
    ev3_motor_config(EV3_MOTOR_RIGHT, LARGE_MOTOR);
    ev3_motor_config(EV3_MOTOR_ARM, LARGE_MOTOR);//MEDIUMとどちらがいいか要検証
}

/**
 * @brief   センサーの設定を行う
 * @return  なし
*/
void configure_sensors()
{
    ev3_sensor_config(EV3_SENSOR_TOUCH, TOUCH_SENSOR);
    ev3_sensor_config(EV3_SENSOR_COLOR, COLOR_SENSOR);
    ev3_sensor_config(EV3_SENSOR_GYRO, GYRO_SENSOR);
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
    while(!ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH));
    while(ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH));
    int color = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);
    return  color;
}
