/**
 * @file    logSettings.cpp
 * @brief   This file has the settings for logging.
 */
#include "logSettings.h"
#include "ValueArray.h"

//! \addtogroup ポートの設定
//@{
//! ログを出力しない
const uint8_t OUTPUT_TYPE_NONE = 0;

//! ログをファイルに出力する場合に指定する
const uint8_t OUTPUT_TYPE_FILE = (1 << 0);

//! ログをLCDに出力する場合に指定する
const uint8_t OUTPUT_TYPE_LCD = (1 << 1);
//@}

//! \addtogroup ログ出力用のmap
//@{
//! ログに出力する文字列
ValueArray* valueArrayType = NULL;

//! ログ出力間隔[単位 : ms] 0の場合は、常に出力する
ValueArray* valueArrayInterval = NULL;

//! 前回ログをストックしたシステム時刻[単位 : ms]
ValueArray* valueArrayLastTime = NULL;
//@}

/**
 *  @brief  ログ用のmapの初期化をおこなう
 *  @param  logType ログの種類
 *  @param  logName ログの種類の名前
 *  @param  interval ログをストックするインターバル[単位 : ms]
 *  @param  lastOutput  最後にログをストックしたシステム時刻[単位 : ms]
 *  @return なし
*/
void initialize_logSetting_map(uint_t logType, char* logName, SYSTIM interval = 0, SYSTIM lastOutput = 0)
{
    valueArrayType->setStringValue(logType, logName);
    valueArrayInterval->setNumberValue(logType, interval);
    valueArrayLastTime->setNumberValue(logType, lastOutput);
}

/**
    @brief  logSettingsの初期化をおこなう
    @return なし
*/
void initialize_logSetting()
{
    if (valueArrayType == NULL) {
        valueArrayType = new ValueArray();
    }

    if (valueArrayInterval == NULL) {
        valueArrayInterval = new ValueArray();
    }

    if (valueArrayLastTime == NULL) {
        valueArrayLastTime = new ValueArray();
    }

    initialize_logSetting_map(LOG_EMERG, (char*)"Shutdown");
    initialize_logSetting_map(LOG_ALERT, (char*)"Alert");
    initialize_logSetting_map(LOG_CRIT, (char*)"CRITICAL");
    initialize_logSetting_map(LOG_ERROR, (char*)"Error");
    initialize_logSetting_map(LOG_WARNING, (char*)"Warning");
    initialize_logSetting_map(LOG_NOTICE, (char*)"Notice");
    initialize_logSetting_map(LOG_INFO, (char*)"Info");
    initialize_logSetting_map(LOG_DEBUG, (char*)"Debug");
    initialize_logSetting_map(LOG_TYPE_GYRO, (char*)"Gyro");
    initialize_logSetting_map(LOG_TYPE_SONAR, (char*)"Sonar", 100);
    initialize_logSetting_map(LOG_TYPE_PID, (char*)"PID");
    initialize_logSetting_map(LOG_TYPE_COLOR, (char*)"Color");
    initialize_logSetting_map(LOG_TYPE_COLOR_BLACK, (char*)"Color(Black)");
    initialize_logSetting_map(LOG_TYPE_COLOR_WHITE, (char*)"Color(White)");
    initialize_logSetting_map(LOG_TYPE_DISTANCE, (char*)"Distance", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_SCENARIO, (char*)"Distance(Scenario)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_TOTAL, (char*)"Distance(Total)", 500);
    initialize_logSetting_map(LOG_TYPE_DIRECTION, (char*)"Direction", 100);
    initialize_logSetting_map(LOG_TYPE_DIRECTION_SCENARIO, (char*)"Direction(Scenario)", 100);
    initialize_logSetting_map(LOG_TYPE_DIRECTION_TOTAL, (char*)"Direction(Total)", 500);
    initialize_logSetting_map(LOG_TYPE_GYRO, (char*)"Gyro");
    initialize_logSetting_map(LOG_TYPE_WRITE_PROCESSING, (char*)"Writing Log");
    initialize_logSetting_map(LOG_TYPE_DISTANCE_RIGHT, (char*)"Distance(Right)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_RIGHT_TOTAL, (char*)"Distance(Right/Total)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_LEFT, (char*)"Distance(Left)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_LEFT_TOTAL, (char*)"Distance(Left/Total)", 100);
    initialize_logSetting_map(LOG_TYPE_SCENARIO, (char*)"Scenario");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_DISTANCE, (char*)"Scenario(distance)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_DIRECTION, (char*)"Scenario(direction)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_POWER, (char*)"Scenario(power)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_PATTERN, (char*)"Scenario(pattern)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_STOP, (char*)"Scenario(stop)");
    initialize_logSetting_map(LOG_TYPE_BATTERY_mA, (char*)"mA");
    initialize_logSetting_map(LOG_TYPE_BATTERY_mV, (char*)"mV");
    initialize_logSetting_map(LOG_TYPE_INITIALIZE, (char*)"Initialize");
    initialize_logSetting_map(LOG_TYPE_CORRECTED_RATIO_LEFT, (char*)"CorrectedRatioLeft", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_RATIO_RIGHT, (char*)"CorrectedRatioRight", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_POWER_LEFT, (char*)"CorrectedPowLeft", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_POWER_RIGHT, (char*)"CorrectedPowRight", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED, (char*)"Average(Speed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE, (char*)"Average(Distance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME, (char*)"Average(Time)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED_LEFT, (char*)"Average(LSpeed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE_LEFT, (char*)"Average(LDistance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME_LEFT, (char*)"Average(LTime)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED_RIGHT, (char*)"Average(RSpeed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE_RIGHT, (char*)"Average(RDistance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME_RIGHT, (char*)"Average(RTime)", 100);
    initialize_logSetting_map(LOG_TYPE_POWER_FOR_CURVE_LEFT, (char*)"PowLeftForCurve", 100);
    initialize_logSetting_map(LOG_TYPE_POWER_FOR_CURVE_RIGHT, (char*)"PowRightForCurve", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_REAL_X, (char*)"Position.x", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_REAL_Y, (char*)"Position.y", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_MAP_X, (char*)"MAP.x", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_MAP_Y, (char*)"MAP.y", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_DIRECTION, (char*)"Direction.ev3", 100);
    initialize_logSetting_map(LOG_TYPE_TEST1, (char*)"Test1", 100);
    initialize_logSetting_map(LOG_TYPE_TEST2, (char*)"Test2", 100);
    initialize_logSetting_map(LOG_TYPE_TEST3, (char*)"Test3", 100);
    initialize_logSetting_map(LOG_TYPE_TEST4, (char*)"Test4", 100);
    initialize_logSetting_map(LOG_TYPE_TEST5, (char*)"Test5", 100);
    initialize_logSetting_map(LOG_TYPE_TEST6, (char*)"Test6", 100);
    initialize_logSetting_map(LOG_TYPE_TEST7, (char*)"Test7", 100);
    initialize_logSetting_map(LOG_TYPE_TEST8, (char*)"Test8", 100);
}

/**
 *  @brief  ログの種類の名前を取得する
 *  @param  logType ログの種類
 *  @return ログの種類(文字列)
*/
char* getLogName(uint_t logType)
{
    return  valueArrayType->getStringValue(logType);
}

/**
 *  @brief  最後にログをストックしたシステム時刻を取得する
 *  @param  logType ログの種類
 *  @return 最後にログをストックしたシステム時刻[単位 : ms]
*/
SYSTIM getLogLastTime(uint_t logType)
{
    return  valueArrayLastTime->getNumberValue(logType);
}

/**
 *  @brief  ログをストックするインターバルを取得する
 *  @param  logType ログの種類
 *  @return ログをストックするインターバル[単位 : ms]
*/
SYSTIM getLogInterval(uint_t logType)
{
    return  valueArrayInterval->getNumberValue(logType);
}

/**
    @brief  ログの種類のLastTimeをセットする
    @param ログの種類
    @param 最後の時間
    @return なし
*/
void setLogLastTime(uint_t logType, SYSTIM lastTime)
{
    valueArrayLastTime->setNumberValue(logType, lastTime);
}
