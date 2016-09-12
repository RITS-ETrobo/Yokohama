/**
 * @file    GyroSensorController.h
 * @brief   ジャイロセンサーを制御する
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "portSettings.h"

//! Class for gyro sensor
class GyroSensorController
{
public:
    explicit GyroSensorController(sensor_port_t _port);
    virtual bool initialize();
    virtual void reset();
    virtual void setEnabledGyroSensor(bool _enabledGyroSensor = true);
    virtual bool isEnabledGyroSensor();
    virtual void updateGyroRate();
    virtual int16_t getGyroRate();
    virtual bool isStaged();

private:
    //! ジャイロセンサーのポート番号
    sensor_port_t port;

    //! ジャイロセンサーの有効性
    bool enabledGyroSensor;

    //! ステージ上にいるか判定
    bool staged;

    //! ジャイロの角速度
    int16_t gyroRate;

    //! 段差乗り上げの際に、ジャイロセンサーが受け取る角速度[度/秒]の閾値。(ただし、約25ms間隔で計測した場合のもの。)
    const int16_t STAGE_ON_THRESHOLD = -90;

};
