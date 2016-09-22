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

//! Class for DistanceRunScenario
class DistanceRunScenario : public Scenario
{
public:
    explicit DistanceRunScenario(int power, float stopDistance);

    void initialize();

    //! 実行
    void Act();

    //! ストップ条件
    bool Stop();

protected:

private:
    int power;
    float stopDistance;
    bool runStop;
    float directionLast;
    float distanceDelta;
    float directionDelta;
    

    DriveController *driveController;

    //@}
};