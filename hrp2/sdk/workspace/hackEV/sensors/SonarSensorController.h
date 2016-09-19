/**
 * @file    SonarSensorController.h
 * @brief   This file has SonarSensorController
 */
#pragma once

#include "product.h"
#include "portSettings.h"
#include <vector>

class SonarSensorController
{
public:
    explicit SonarSensorController(sensor_port_t _port);

    virtual bool initialize();
    virtual void setEnabled(bool _enabled = true);
    virtual bool isEnabled() { return enabled; };
    virtual int16_t executeSonar();
    virtual bool isSafe() { return safe; };
    virtual void setThresholdDistance(int16_t thresholdDistance_) { thresholdDistance = thresholdDistance_; };
    virtual int16_t getThresholdDistance() { return thresholdDistance; };

#ifndef EV3_UNITTEST
    virtual void confirm(int16_t distance);
#endif  //  EV3_UNITTEST

private:
    void checkSafe(int16_t distance);

    //! 超音波センサーのポート
    sensor_port_t   port;

    //! trueの場合に、超音波センサーを使用するかどうか
    bool    enabled;

    //! 初期化済み判定フラグ
    bool    initialized;

    //! レールを跨げるかどうか
    bool    safe;

    //! 板があるかどうかの距離の閾値(目標までの距離により、可変)
    int16_t thresholdDistance;

    //! 板が通過した後に、走行体がレールを通過可能な時間(新幹線の速度により、可変)
    int16_t thresholdDuration;

    //! 新幹線がレールを1周する時間[単位 : ms]
    SYSTIM  lapShinkansen;
};
