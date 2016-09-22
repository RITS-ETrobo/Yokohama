/**
 * @file    Scenario.h
 * @brief   シナリオの親クラス
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"

//! Class for driving
class Scenario
{
public:
    virtual void initialize()=0;

    //! 実行
    virtual void Act()=0;

    //! ストップ条件
    virtual bool Stop()=0;

private:
    //! 前回の向きの累積[単位 : 度]
    float   directionLast;

    //@}
};
