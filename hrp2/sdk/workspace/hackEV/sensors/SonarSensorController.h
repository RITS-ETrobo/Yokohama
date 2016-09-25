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
    virtual bool catchablePrizeSafety();

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
    const int16_t SAFETY_CATCHABLE_PRIZE_DISTANCE_MIN = 3;
    //! 検証を安全につかめる最大距離
    const int16_t SAFETY_CATCHABLE_PRIZE_DISTANCE_MAX = 3;
};
