/**
 * @file    logSettings.h
 * @brief   This file has the settings for logging.
 */
#pragma once

#include "product.h"
#include <string>

//! \addtogroup ログ出力の設定
//@{
//! ログを出力しない
extern const uint8_t OUTPUT_TYPE_NONE;

//! ログをファイルに出力する場合に指定する
extern const uint8_t OUTPUT_TYPE_FILE;

//! ログをLCDに出力する場合に指定する
extern const uint8_t OUTPUT_TYPE_LCD;
//@}

//! \addtogroup ログの種類(hrp2/include/t_syslog.h での定義も確認する事)
//@{
typedef enum
{
    //! PID値に関するログ
    LOG_TYPE_PID = 0x81,

    //! 色(多色)に関するログ
    LOG_TYPE_COLOR,

    //! 色(黒)に関するログ
    LOG_TYPE_COLOR_BLACK,

    //! 色(白)に関するログ
    LOG_TYPE_COLOR_WHITE,

    //! 距離(瞬間)
    LOG_TYPE_DISTANCE,

    //! 距離(累積/シナリオ)
    LOG_TYPE_DISTANCE_SCENARIO,

    //! 距離(累積)
    LOG_TYPE_DISTANCE_TOTAL,

    //! 角度(瞬間)
    LOG_TYPE_DIRECTION,

    //! 角度(累積/シナリオ)
    LOG_TYPE_DIRECTION_SCENARIO,

    //! 角度(累積)
    LOG_TYPE_DIRECTION_TOTAL,

    //! ジャイロセンサー
    LOG_TYPE_GYRO,

    //! 超音波センサー
    LOG_TYPE_SONAR,

    //! ログを書き出している時間
    LOG_TYPE_WRITE_PROCESSING,

    //! 距離(右瞬間)
    LOG_TYPE_DISTANCE_RIGHT,

    //! 距離(右累積)
    LOG_TYPE_DISTANCE_RIGHT_TOTAL,

    //! 距離(左瞬間)
    LOG_TYPE_DISTANCE_LEFT,

    //! 距離(左累積)
    LOG_TYPE_DISTANCE_LEFT_TOTAL,

    //! シナリオ情報
    LOG_TYPE_SCENARIO,

    //! バッテリー情報(mA)
    LOG_TYPE_BATTERY_mA,

    //! バッテリー情報(mV)
    LOG_TYPE_BATTERY_mV,

    //! シナリオ情報(距離[単位 : cm])
    LOG_TYPE_SCENARIO_DISTANCE,

    //! シナリオ情報(向き[単位 : 度])
    LOG_TYPE_SCENARIO_DIRECTION,

    //! シナリオ情報(入力)
    LOG_TYPE_SCENARIO_POWER,

    //! シナリオ情報(走行シナリオの順番)
    LOG_TYPE_SCENARIO_PATTERN,

    //! シナリオ情報(走行シナリオ完了後にストップするか)
    LOG_TYPE_SCENARIO_STOP,

    //! 初期化
    LOG_TYPE_INITIALIZE,

    //! 補正した左モーターの補正値
    LOG_TYPE_CORRECTED_RATIO_LEFT,

    //! 補正した右モーターの補正値
    LOG_TYPE_CORRECTED_RATIO_RIGHT,

    //! 補正した左モーターの出力値
    LOG_TYPE_CORRECTED_POWER_LEFT,

    //! 補正した右モーターの出力値
    LOG_TYPE_CORRECTED_POWER_RIGHT,

    //! 平均速度
    LOG_TYPE_AVERAGE_SPEED,

    //! 平均速度(距離)
    LOG_TYPE_AVERAGE_DISTANCE,

    //! 平均速度(時間)
    LOG_TYPE_AVERAGE_TIME,

    //! 平均速度(左ホイール)
    LOG_TYPE_AVERAGE_SPEED_LEFT,

    //! 平均速度(左ホイール距離)
    LOG_TYPE_AVERAGE_DISTANCE_LEFT,

    //! 平均速度(左ホイール時間)
    LOG_TYPE_AVERAGE_TIME_LEFT,

    //! 平均速度(右ホイール)
    LOG_TYPE_AVERAGE_SPEED_RIGHT,

    //! 平均速度(右ホイール距離)
    LOG_TYPE_AVERAGE_DISTANCE_RIGHT,

    //! 平均速度(右ホイール時間)
    LOG_TYPE_AVERAGE_TIME_RIGHT,

    //! カーブするために調整した左モーターのパワー
    LOG_TYPE_POWER_FOR_CURVE_LEFT,

    //! カーブするために調整した右モーターのパワー
    LOG_TYPE_POWER_FOR_CURVE_RIGHT,

    //! 走行体の実際のX座標[単位 : cm]
    LOG_TYPE_EV3_POSITION_REAL_X,

    //! 走行体の実際のY座標[単位 : cm]
    LOG_TYPE_EV3_POSITION_REAL_Y,

    //! 走行体のマップ上のX座標[単位 : ピクセル]
    LOG_TYPE_EV3_POSITION_MAP_X,

    //! 走行体のマップ上のY座標[単位 : ピクセル]
    LOG_TYPE_EV3_POSITION_MAP_Y,

    //! 走行体の向き[単位 : 度]
    LOG_TYPE_EV3_DIRECTION,

    //! テスト用1
    LOG_TYPE_TEST1,

    //! テスト用2
    LOG_TYPE_TEST2,

    //! テスト用3
    LOG_TYPE_TEST3,

    //! テスト用4
    LOG_TYPE_TEST4,

    //! テスト用5
    LOG_TYPE_TEST5,

    //! テスト用6
    LOG_TYPE_TEST6,

    //! テスト用7
    LOG_TYPE_TEST7,

    //! テスト用8
    LOG_TYPE_TEST8
>>>>>>> origin/hackev/cpp/develop
} LOGTYPE;
//@}

extern void initialize_logSetting();
extern std::string getLogName(uint_t logType);
void initialize_logSetting_map(uint_t logType, std::string logName, SYSTIM interval = 0, SYSTIM lastOutput = 0);
extern SYSTIM getLogLastTime(uint_t logType);
extern SYSTIM getLogInterval(uint_t logType);
extern void setLogLastTime(uint_t logType, SYSTIM lastTime);
