/**
 * @file    MotorWheel.h
 * @brief   タイヤについているモーターを制御する
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "portSettings.h"
#include "SpeedCalculator.h"

//! Class for MotorWheel
class MotorWheel
{
public:
    explicit MotorWheel(motor_port_t portMotor);

    virtual bool initialize();
    virtual ER run(int power);
    virtual ER stop(bool_t brake = true);

    virtual void updateDistance(); 
    virtual float getDistance();
    virtual float getDistanceDelta();
    virtual int getCurrentPower();

private:
    //! リセットしてから、どれくらい走ったか。前回までにホイールが移動した位置[単位 : cm]
    float   distanceLast;

    //! 現在までにホイールが移動した位置[単位 : cm]
    float   distance;

    //! 対象のポート
    motor_port_t    portMotor;

    //! \addtogroup 速度測定用のインスタンス
    //@{
    //! 最近の100ms
    SpeedCalculator *speedCalculator100ms;

    //! 現在のモーターのパワー
    int currentPower;
    //@}
};
