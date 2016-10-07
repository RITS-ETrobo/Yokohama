/**
 * @file    SonarSensorController.h
 * @brief   This file has SonarSensorController
 */
#pragma once

#include "product.h"
#include "portSettings.h"

class SonarSensorController
{
public:
    explicit SonarSensorController(sensor_port_t _port);

    virtual bool initialize();
    virtual void setEnabled(bool _enabled = true);
    virtual bool isEnabled() { return enabled; };
    virtual int16_t executeSonar();
    virtual bool isGrabbableDistance();
    virtual void stoppingPassShinkansen();

#ifndef EV3_UNITTEST
    virtual void confirm(int16_t distance);
#endif  //  EV3_UNITTEST

private:
    //! 超音波センサーのポート
    sensor_port_t   port;

    //! trueの場合に、超音波センサーを使用するかどうか
    bool    enabled;

    //! 初期化済み判定フラグ
    bool    initialized;

    //! 検証を安全につかめる最小距離
    const int16_t SAFETY_CATCHABLE_PRIZE_DISTANCE_MIN;

    //! 検証を安全につかめる最大距離
    const int16_t SAFETY_CATCHABLE_PRIZE_DISTANCE_MAX;

    //! 新幹線が目の前にいると判断する閾値 新幹線が向こう側にいるときは70なので、30以下であれば十分に目の前にいることになる。
    const int16_t SHINKANSEN_INFRONT = 30;
};
