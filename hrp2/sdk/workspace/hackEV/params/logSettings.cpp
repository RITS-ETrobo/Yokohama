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

//! ジャイロ
const uint_t LOG_TYPE_GYRO = 0x87;

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
//@}

std::map<uint_t, char*> LOG_TYPE_MAP;

/**
    @brief  logSettingsの初期化をおこなう
    @return なし
*/
void initialize_logSetting()
{
    LOG_TYPE_MAP[LOG_EMERG] = "Shutdown";
    LOG_TYPE_MAP[LOG_ALERT] = "Alert";
    LOG_TYPE_MAP[LOG_CRIT] = "CRITICAL";
    LOG_TYPE_MAP[LOG_ERROR] = "Error";
    LOG_TYPE_MAP[LOG_WARNING] = "Warning";
    LOG_TYPE_MAP[LOG_NOTICE] = "Notice";
    LOG_TYPE_MAP[LOG_INFO] = "Info";
    LOG_TYPE_MAP[LOG_DEBUG] = "Debug";
    LOG_TYPE_MAP[LOG_TYPE_GYRO] = "Gyro";
    LOG_TYPE_MAP[LOG_TYPE_PID] = "PID";
    LOG_TYPE_MAP[LOG_TYPE_COLOR] = "Color";
    LOG_TYPE_MAP[LOG_TYPE_COLOR_BLACK] = "Color(Black)";
    LOG_TYPE_MAP[LOG_TYPE_COLOR_WHITE] = "Color(White)";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE] = "Distance";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE_TOTAL] = "Distance(Total)";
    LOG_TYPE_MAP[LOG_TYPE_DIRECTION] = "Direction";
    LOG_TYPE_MAP[LOG_TYPE_DIRECTION_TOTAL] = "Direction(Total)";
    LOG_TYPE_MAP[LOG_TYPE_GYRO] = "Gyro";
    LOG_TYPE_MAP[LOG_TYPE_WRITE_PROCESSING] = "Writing Log";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE_RIGHT] = "Distance(Right)";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE_RIGHT_TOTAL] = "Distance(Right/Total)";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE_LEFT] = "Distance(Left)";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE_LEFT_TOTAL] = "Distance(Left/Total)";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO] = "Scenario";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO_DISTANCE] = "Scenario(distance)";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO_DIRECTION] = "Scenario(direction)";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO_POWER] = "Scenario(power)";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO_PATTERN] = "Scenario(pattern)";
    LOG_TYPE_MAP[LOG_TYPE_SCENARIO_STOP] = "Scenario(stop)";
    LOG_TYPE_MAP[LOG_TYPE_BATTERY_mA] = "mA";
    LOG_TYPE_MAP[LOG_TYPE_BATTERY_mV] = "mV";
    LOG_TYPE_MAP[LOG_TYPE_INITIALIZE] = "Initialize";
}

/**
    @brief  ログの種類の名前を取得する
    @return ログの種類(文字列)
*/
char* getLogName(uint_t logType)
{
    return LOG_TYPE_MAP[logType];
}
