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

//! 色(白黒)に関するログ
const uint_t LOG_TYPE_COLOR_BW = 0x83;

//! 距離(瞬間)
const uint_t LOG_TYPE_DISTANCE = 0x83;

//! 距離(累積)
const uint_t LOG_TYPE_DISTANCE_STORED = 0x84;

//! 角度(瞬間)
const uint_t LOG_TYPE_DIRECTION = 0x85;

//! 角度(累積)
const uint_t LOG_TYPE_DIRECTION_STORED = 0x86;

//! ジャイロ
const uint_t LOG_TYPE_GYRO = 0x87;
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
    LOG_TYPE_MAP[LOG_TYPE_COLOR_BW] = "Color(B/W)";
    LOG_TYPE_MAP[LOG_TYPE_DISTANCE] = "Distance";
    LOG_TYPE_MAP[LOG_TYPE_DIRECTION] = "Direction";
    LOG_TYPE_MAP[LOG_TYPE_DIRECTION_STORED] = "Direction(Stored)";
    LOG_TYPE_MAP[LOG_TYPE_GYRO] = "Gyro";
}

/**
    @brief  ログの種類の名前を取得する
    @return ログの種類(文字列)
*/
char* getLogName(uint_t logType)
{
    return LOG_TYPE_MAP[logType];
}
