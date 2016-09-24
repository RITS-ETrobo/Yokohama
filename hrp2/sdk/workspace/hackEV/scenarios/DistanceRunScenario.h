/**
 * @file    DistanceRunScenario.h
 * @brief   走行シナリオクラス
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"
#include "Scenario.h"
#include "instances.h"

//! Class for DistanceRunScenario
class DistanceRunScenario : public Scenario
{
public:
    explicit DistanceRunScenario(int power, float stopDistance, bool finishedStop);

    void initialize();

    //! 実行
    void Act();

    //! ストップ条件
    bool Stop();

protected:

private:
    int power;
    float stopDistance;
    bool finishedStop;
    float directionLast;
    float distanceDelta;
    float directionDelta;

    //@}
};