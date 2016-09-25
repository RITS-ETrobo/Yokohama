/**
 * @file    logSettings.cpp
 * @brief   This file has the settings for logging.
 */
#include "logSettings.h"
#include <map>

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
std::map<uint_t, std::string> LOG_TYPE_MAP;

//! 前回ログをストックしたシステム時刻[単位 : ms]
std::map<uint_t, SYSTIM> LOG_TYPE_LASTTIME_MAP;

//! ログ出力間隔[単位 : ms] 0の場合は、常に出力する
std::map<uint_t, SYSTIM> LOG_TYPE_INTERVAL_MAP;
//@}

/**
    @brief  logSettingsの初期化をおこなう
    @return なし
*/
void initialize_logSetting()
{
    initialize_logSetting_map(LOG_EMERG, "Shutdown");
    initialize_logSetting_map(LOG_ALERT, "Alert");
    initialize_logSetting_map(LOG_CRIT, "CRITICAL");
    initialize_logSetting_map(LOG_ERROR, "Error");
    initialize_logSetting_map(LOG_WARNING, "Warning");
    initialize_logSetting_map(LOG_NOTICE, "Notice");
    initialize_logSetting_map(LOG_INFO, "Info");
    initialize_logSetting_map(LOG_DEBUG, "Debug");
    initialize_logSetting_map(LOG_TYPE_GYRO, "Gyro");
    initialize_logSetting_map(LOG_TYPE_SONAR, "Sonar", 100);
    initialize_logSetting_map(LOG_TYPE_PID, "PID");
    initialize_logSetting_map(LOG_TYPE_COLOR, "Color");
    initialize_logSetting_map(LOG_TYPE_COLOR_BLACK, "Color(Black)");
    initialize_logSetting_map(LOG_TYPE_COLOR_WHITE, "Color(White)");
    initialize_logSetting_map(LOG_TYPE_DISTANCE, "Distance", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_TOTAL, "Distance(Total)", 100);
    initialize_logSetting_map(LOG_TYPE_DIRECTION, "Direction", 100);
    initialize_logSetting_map(LOG_TYPE_DIRECTION_TOTAL, "Direction(Total)", 100);
    initialize_logSetting_map(LOG_TYPE_GYRO, "Gyro");
    initialize_logSetting_map(LOG_TYPE_WRITE_PROCESSING, "Writing Log");
    initialize_logSetting_map(LOG_TYPE_DISTANCE_RIGHT, "Distance(Right)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_RIGHT_TOTAL, "Distance(Right/Total)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_LEFT, "Distance(Left)", 100);
    initialize_logSetting_map(LOG_TYPE_DISTANCE_LEFT_TOTAL, "Distance(Left/Total)", 100);
    initialize_logSetting_map(LOG_TYPE_SCENARIO, "Scenario");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_DISTANCE, "Scenario(distance)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_DIRECTION, "Scenario(direction)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_POWER, "Scenario(power)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_PATTERN, "Scenario(pattern)");
    initialize_logSetting_map(LOG_TYPE_SCENARIO_STOP, "Scenario(stop)");
    initialize_logSetting_map(LOG_TYPE_BATTERY_mA, "mA");
    initialize_logSetting_map(LOG_TYPE_BATTERY_mV, "mV");
    initialize_logSetting_map(LOG_TYPE_INITIALIZE, "Initialize");
    initialize_logSetting_map(LOG_TYPE_CORRECTED_RATIO_LEFT, "CorrectedRatioLeft", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_RATIO_RIGHT, "CorrectedRatioRight", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_POWER_LEFT, "CorrectedPowLeft", 100);
    initialize_logSetting_map(LOG_TYPE_CORRECTED_POWER_RIGHT, "CorrectedPowRight", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED, "Average(Speed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE, "Average(Distance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME, "Average(Time)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED_LEFT, "Average(LSpeed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE_LEFT, "Average(LDistance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME_LEFT, "Average(LTime)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_SPEED_RIGHT, "Average(RSpeed)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_DISTANCE_RIGHT, "Average(RDistance)", 100);
    initialize_logSetting_map(LOG_TYPE_AVERAGE_TIME_RIGHT, "Average(RTime)", 100);
    initialize_logSetting_map(LOG_TYPE_POWER_FOR_CURVE_LEFT, "PowLeftForCurve", 100);
    initialize_logSetting_map(LOG_TYPE_POWER_FOR_CURVE_RIGHT, "PowRightForCurve", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_REAL_X, "Position.x", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_REAL_Y, "Position.y", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_MAP_X, "MAP.x", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_POSITION_MAP_Y, "MAP.y", 100);
    initialize_logSetting_map(LOG_TYPE_EV3_DIRECTION, "Direction.ev3", 100);
}

/**
 *  @brief  ログ用のmapの初期化をおこなう
 *  @param  logType ログの種類
 *  @param  logName ログの種類の名前
 *  @param  interval ログをストックするインターバル[単位 : ms]
 *  @param  lastOutput  最後にログをストックしたシステム時刻[単位 : ms]
 *  @return なし
*/
void initialize_logSetting_map(uint_t logType, std::string logName, SYSTIM interval /*= 0*/, SYSTIM lastOutput /*= 0*/)
{
    LOG_TYPE_MAP[logType] = logName;
    LOG_TYPE_INTERVAL_MAP[logType] = interval;
    LOG_TYPE_LASTTIME_MAP[logType] = lastOutput;
}

/**
 *  @brief  ログの種類の名前を取得する
 *  @param  logType ログの種類
 *  @return ログの種類(文字列)
*/
std::string getLogName(uint_t logType)
{
    return LOG_TYPE_MAP[logType];
}

/**
 *  @brief  最後にログをストックしたシステム時刻を取得する
 *  @param  logType ログの種類
 *  @return 最後にログをストックしたシステム時刻[単位 : ms]
*/
SYSTIM getLogLastTime(uint_t logType)
{
    return LOG_TYPE_LASTTIME_MAP[logType];
}

/**
 *  @brief  ログをストックするインターバルを取得する
 *  @param  logType ログの種類
 *  @return ログをストックするインターバル[単位 : ms]
*/
SYSTIM getLogInterval(uint_t logType)
{
    return LOG_TYPE_INTERVAL_MAP[logType];
}

/**
    @brief  ログの種類のLastTimeをセットする
    @param ログの種類
    @param 最後の時間

    @return なし
*/
void setLogLastTime(uint_t logType, SYSTIM lastTime)
{
    LOG_TYPE_LASTTIME_MAP[logType] = lastTime;
}
