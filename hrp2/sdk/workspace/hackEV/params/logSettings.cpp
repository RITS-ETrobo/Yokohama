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

//! \addtogroup ログの種類(hrp2/include/t_syslog.h での定義も確認する事)
//@{
//! PID値に関するログ
const uint_t LOG_TYPE_PID = 0x81;

//! 色(多色)に関するログ
const uint_t LOG_TYPE_COLOR = 0x82;

//! 色(黒)に関するログ
const uint_t LOG_TYPE_COLOR_BLACK = 0x89;

//! 色(白)に関するログ
const uint_t LOG_TYPE_COLOR_WHITE = 0x8A;

//! 距離(瞬間)
const uint_t LOG_TYPE_DISTANCE = 0x83;

//! 距離(累積)
const uint_t LOG_TYPE_DISTANCE_TOTAL = 0x84;

//! 角度(瞬間)
const uint_t LOG_TYPE_DIRECTION = 0x85;

//! 角度(累積)
const uint_t LOG_TYPE_DIRECTION_TOTAL = 0x86;

//! ジャイロセンサー
const uint_t LOG_TYPE_GYRO = 0x87;

//! 超音波センサー
const uint_t LOG_TYPE_SONAR = 0xA5;

//! ログを書き出している時間
const uint_t LOG_TYPE_WRITE_PROCESSING = 0x88;

//! 距離(右瞬間)
const uint_t LOG_TYPE_DISTANCE_RIGHT = 0x8B;

//! 距離(右累積)
const uint_t LOG_TYPE_DISTANCE_RIGHT_TOTAL = 0x8C;

//! 距離(左瞬間)
const uint_t LOG_TYPE_DISTANCE_LEFT = 0x8D;

//! 距離(左累積)
const uint_t LOG_TYPE_DISTANCE_LEFT_TOTAL = 0x8E;

//! シナリオ情報
extern const uint_t LOG_TYPE_SCENARIO = 0x8F;

//! バッテリー情報(mA)
const uint_t LOG_TYPE_BATTERY_mA = 0x90;

//! バッテリー情報(mV)
const uint_t LOG_TYPE_BATTERY_mV = 0x91;

//! シナリオ情報(距離[単位 : cm])
const uint_t LOG_TYPE_SCENARIO_DISTANCE = 0x92;

//! シナリオ情報(向き[単位 : 度])
const uint_t LOG_TYPE_SCENARIO_DIRECTION = 0x93;

//! シナリオ情報(入力)
const uint_t LOG_TYPE_SCENARIO_POWER = 0x94;

//! シナリオ情報(走行シナリオの順番)
const uint_t LOG_TYPE_SCENARIO_PATTERN = 0x95;

//! シナリオ情報(走行シナリオ完了後にストップするか)
const uint_t LOG_TYPE_SCENARIO_STOP = 0x96;

//! 初期化
const uint_t LOG_TYPE_INITIALIZE = 0x97;

//! 補正した左モーターの補正値
const uint_t LOG_TYPE_CORRECTED_RATIO_LEFT = 0x98;

//! 補正した右モーターの補正値
const uint_t LOG_TYPE_CORRECTED_RATIO_RIGHT = 0x99;

//! 補正した左モーターの出力値
const uint_t LOG_TYPE_CORRECTED_POWER_LEFT = 0x9A;

//! 補正した右モーターの出力値
const uint_t LOG_TYPE_CORRECTED_POWER_RIGHT = 0x9B;

//! 平均速度
const uint_t LOG_TYPE_AVERAGE_SPEED = 0x9C;

//! 平均速度(距離)
const uint_t LOG_TYPE_AVERAGE_DISTANCE = 0x9D;

//! 平均速度(時間)
const uint_t LOG_TYPE_AVERAGE_TIME = 0x9E;

//! 平均速度(左ホイール)
const uint_t LOG_TYPE_AVERAGE_SPEED_LEFT = 0x9F;

//! 平均速度(左ホイール距離)
const uint_t LOG_TYPE_AVERAGE_DISTANCE_LEFT = 0xA0;

//! 平均速度(左ホイール時間)
const uint_t LOG_TYPE_AVERAGE_TIME_LEFT = 0xA1;

//! 平均速度(右ホイール)
const uint_t LOG_TYPE_AVERAGE_SPEED_RIGHT = 0xA2;

//! 平均速度(右ホイール距離)
const uint_t LOG_TYPE_AVERAGE_DISTANCE_RIGHT = 0xA3;

//! 平均速度(右ホイール時間)
const uint_t LOG_TYPE_AVERAGE_TIME_RIGHT = 0xA4;
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
