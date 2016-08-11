/**
 * @file    MotorWheel.h
 * @brief   タイヤについているモーターを制御する
 */
#pragma once

//! ターゲット依存の定義
#include "ev3api.h"
#include "target_test.h"

#include "portSettings.h"

//! Class for MotorWheel
class MotorWheel
{
public:
    explicit MotorWheel(motor_port_t portMotor);

    virtual void initialize();
    virtual ER stop(bool_t brake = true);

    virtual void updateDistance(); 
    virtual float getDistance();
    virtual float getDistanceDelta();

private:
    //! リセットしてから、どれくらい走ったか。前回までにホイールが移動した位置[単位 : cm]
    float   distanceLast;

    //! 現在までにホイールが移動した位置[単位 : cm]
    float   distance;

    //! 対象のポート
    motor_port_t    portMotor;
};
