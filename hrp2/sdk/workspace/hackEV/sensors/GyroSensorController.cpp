/**
 * @file    GyroSensorController.cpp
 * @brief   ジャイロセンサーを制御する
 */
#include "instances.h"
#include "utilities.h"

#include "logSettings.h"



//! class for gyro sensor
GyroSensorController::GyroSensorController(sensor_port_t _port)
    : port(_port)
{
}

/**
 *  @brief  初期化処理
 *  @return true : 成功
 *  @return false : 失敗
*/
bool GyroSensorController::initialize()
{
    ev3_sensor_config(port, GYRO_SENSOR);
    reset();
    if (ev3_gyro_sensor_reset(port) != E_OK) {
        logger->addLog(LOG_NOTICE, "GYRO ER");
        return false;
    }

    return  true;
}

/**
 *  @brief  値のリセット
 *  @return なし
*/
void GyroSensorController::reset()
{
    setEnabledGyroSensor(false);
    staged = false;
    gyroRate = 0;
}

/**
 * @brief   ジャイロセンサーの有効・無効をセットする
 * @param   _enabledGyroSensor  @a true 有効, @a false 無効
 * @return  なし
 */
void GyroSensorController::setEnabledGyroSensor(bool _enabledGyroSensor)
{
    enabledGyroSensor = _enabledGyroSensor;
}

/**
 * @brief   ジャイロセンサーの有効・無効を取得する
 * @return  true:有効
 *          false:無効
 */
bool GyroSensorController::isEnabledGyroSensor()
{
    return enabledGyroSensor;
}

/**
 * @brief   ジャイロセンサーの角速度を更新する
 * @return  ジャイロセンサーの角速度 [度/秒]
*/
void GyroSensorController::updateGyroRate()
{
    gyroRate = ev3_gyro_sensor_get_rate(port);
}

/**
 * @brief   ジャイロセンサーの角速度を取得する
 * @return  ジャイロセンサーの角速度 [度/秒]
*/
int16_t GyroSensorController::getGyroRate()
{
    return gyroRate;
}

/**
 * @brief   ステージに乗ったかどうかのジャイロ判定を更新する。
 * @return  なし
 */
bool GyroSensorController::isStaged()
{
    gyroRate = ev3_gyro_sensor_get_rate(port);
    staged = (gyroRate < STAGE_ON_THRESHOLD);
    return staged;
}


