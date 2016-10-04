/**
 * @file    utilities.cpp
 * @brief   This file has the utilitiy functions.
 */

#include "ev3api.h"
#include "utilities.h"
#include "instances.h"

std::map<runPattern, PID_PARAMETER> PID_MAP;

//! \addtogroup PID制御で用いる変数
//@{
//! 白の光量値
int white = 0;

//! 黒の光量値
int black = 0;
//@}

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

/**
 * @brief 比較対象の絶対値よりも大きい値かどうか
 * @param targetValue 大きさを判定する値
 * @param compareValue 比較対象の値
 * @return  targetValueの絶対値がcompareValueの絶対値よりも大きければtrue,小さければfalse
*/
bool isGreaterAbsoluteValue(float targetValue, float compareValue){
    //! 絶対値を比較して大きければtrueを返す
    if(fabsf(targetValue) >= fabsf(compareValue)){
        return true;
    }
    
    //それ以外はfalseを返す
    return false;
}

/**
 * @brief バッテリーの残電圧と残電流をLCDに出力する
 * @param isOutputLog   trueの場合、ログに出力する
 * @return  なし
*/
void confirmBattery(bool isOutputLog /*= false*/)
{
    char message[16];
    memset(message, '\0', sizeof(message));
    int battery_mA = ev3_battery_current_mA();
    int battery_mV = ev3_battery_voltage_mV();
    sprintf(message, "%04d mA %04d mV", battery_mA, battery_mV); 

    writeStringLCD(message);
    if (isOutputLog && logger) {
        logger->addLogInt(LOG_TYPE_BATTERY_mA, battery_mA);
        logger->addLogInt(LOG_TYPE_BATTERY_mV, battery_mV);
    }
}

/**
 * @brief   白黒のキャリブレートをおこなう
 * @return  なし
*/
void calibrateBW()
{
    char message[16];

    //! Calibrate for light intensity of WHITE
    writeStringLCD("WHITE");
    white = calibrate_light_intensity();
    memset(message, '\0', sizeof(message));
    sprintf(message, "   %03d", white);
    writeStringLCD(message);

    //! Calibrate for light intensity of BLACK
    writeStringLCD("BLACK");
    black = calibrate_light_intensity();
    sprintf(message, "   %03d", black);
    writeStringLCD(message);

    if (logger) {
        logger->addLogInt(LOG_TYPE_COLOR_WHITE, white);
        logger->addLogInt(LOG_TYPE_COLOR_BLACK, black);
    }
}

/**
 * @brief   色情報をLCDに表示する
 * @return  なし
*/
void viewColor()
{
    if (!colorSensorController) {
        writeStringLCD("[ERROR]ColorSensor");
        return;
    }

    ev3_speaker_play_tone(NOTE_F5, 200);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_E6, 200);
    char message[16];

    memset(message, '\0', sizeof(message));
    sprintf(message, " Push Touch Sensor!");
    writeStringLCD(message);

    //! タッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    //! 取得したカラー名をLCDに表示させる
    memset(message, '\0', sizeof(message));
    sprintf(message, "%s", colorSensorController->getColorName().c_str());
    writeStringLCD(message);

    //! RGB値を取得
    rgb_raw_t colorRGB = colorSensorController->getColorRGBraw();

    //! 取得したRGB値をLCDに表示させる
    memset(message, '\0', sizeof(message));
    sprintf(message, "R: %d", colorRGB.r);
    writeStringLCD(message);

    memset(message, '\0', sizeof(message));
    sprintf(message, "G: %d", colorRGB.g);
    writeStringLCD(message);

    memset(message, '\0', sizeof(message));
    sprintf(message, "B: %d", colorRGB.b);
    writeStringLCD(message);
}
