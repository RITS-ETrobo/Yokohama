/**
 * @file    UltrasonicController.cpp
 * @brief   This file has UltrasonicController.
 */

#include "SonarSensorController.h"
#include "instances.h"

SonarSensorController::SonarSensorController(sensor_port_t _port)
    : port(_port)
    , enabled(false)
    , initialized(false)
{
}
/**
 * @brief   超音波センサの初期化処理
 * @return  true : 初期化成功
*/
bool SonarSensorController::initialize()
{
    if (initialized == true) {
        return  true;
    }

    ER  result = E_OK;
#ifndef EV3_UNITTEST
    result = ev3_sensor_config(port, ULTRASONIC_SENSOR);
#endif  //  EV3_UNITTEST
    if (result == E_OK) {
        initialized = true;
    }

    return  false;
}

/**
 * @brief   超音波センサの有効/無効を切り替える
 * @return  なし
*/
void SonarSensorController::setEnabled(bool _enabled /*= true*/)
{
    enabled = _enabled;
}

#ifndef EV3_UNITTEST
/**
 * @brief   LEDの点灯（or消灯）処理で超音波
 * @param   [in] color LEDの色（オフを含む）
 * @param   [in] duration 点灯(or消灯)時間[ms]
 * @return  なし
*/
void SonarSensorController::confirm(int16_t distance)
{
    if (distance <= 0) {
        return;
    }

    //! 距離[cm]*10を点滅間隔とする（最大3秒)
    TMO duration = 10 * distance;
    if (duration > 10 * 300) {
        duration = 10 * 300;
    }

    //! 距離が近いと短周期、遠いと長周期の点滅
    ev3_led_set_color(LED_RED);
    tslp_tsk(duration);

    ev3_led_set_color(LED_OFF);
    tslp_tsk(duration);
}
#endif  //  EV3_UNITTEST

/**
 * @brief   超音波の測定
 * @return  反射したオブジェクトまでの距離[単位 : cm]
*/
int16_t SonarSensorController::executeSonar()
{
    int16_t distance = 0;

#ifndef EV3_UNITTEST
    if (isEnabled()) {
        distance = ev3_ultrasonic_sensor_get_distance(EV3_SENSOR_SONAR);
        if (logger) {
            logger->addLogInt(LOG_TYPE_SONAR, distance);
        }
    }
#endif  //  EV3_UNITTEST

    return  distance;
}

/**
 * @brief   懸賞を安全につかめる距離か検証する
 *
 * @return  つかめる距離ならばtrueを返す
 */
bool SonarSensorController::isGrabbableDistance()
{
    int16_t distance = -1;

    if (isEnabled()) {
        distance = executeSonar();
        if ((SAFETY_CATCHABLE_PRIZE_DISTANCE_MIN <= distance) &&
            (distance <= SAFETY_CATCHABLE_PRIZE_DISTANCE_MAX)) {
            return true;
        }
    }
    return false;
}
