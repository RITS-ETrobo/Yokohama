/**
 * @file    PinWheelScenario.h
 * @brief   その場回転シナリオクラス
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"
#include "Scenario.h"
#include "instances.h"

//! Class for PinWheelScenario
class PinWheelScenario : public Scenario
{
public:
    explicit PinWheelScenario(int power, float stopDirection, bool finishedStop);

    void initialize();

    //! 実行
    void Act();

    //! ストップ条件
    bool Stop();

protected:

private:
    int power;
    float stopDirection;
    bool finishedStop;
    float directionLast;
    float distanceDelta;
    float directionDelta;

    //@}
};