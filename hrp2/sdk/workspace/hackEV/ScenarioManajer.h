/**
 * @file    ScenarioManajer.h
 * @brief   走行シナリオクラス
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"
#include "Scenario.h"
#include <vector>
#include "DistanceRunScenario.h"
#include "PinWheelScenario.h"

using namespace std;

//! Class for ScenarioManajer
class ScenarioManajer
{
public:
    explicit ScenarioManajer();

    void initialize();

    //! シナリオの実行
    void act();

protected:

private:
    vector<Scenario*> ScenarioList;
    
    //@}
};