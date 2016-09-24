/**
 * @file    DistanceRunScenario.cpp
 * @brief   シナリオの親クラス
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "instances.h"
#include "utilities.h"

#include "wheelSettings.h"
#include "logSettings.h"
#include "portSettings.h"

#include "pid_controller.h"

#include "Scenario.h"
#include "DistanceRunScenario.h"

//! 距離まで走るクラス
DistanceRunScenario::DistanceRunScenario(int power, float stopDistance, bool finishedStop)
    : power(power)
    , stopDistance(stopDistance)
    , finishedStop(finishedStop)
    , directionLast(0.0F)
    , distanceDelta(0.0F)
    , directionDelta(0.0F)
    
{
}

/**
 * @brief   初期化
*/
void DistanceRunScenario::initialize(){
    driveController->initialize();
    driveController->setLastWheelPower(power, power);
    if (logger) {
        logger->addLog(LOG_NOTICE, "RunScenario");
        logger->addLogInt(LOG_TYPE_SCENARIO_POWER, power);
        logger->addLogFloat(LOG_TYPE_SCENARIO_DISTANCE, stopDistance);
        logger->addLogInt(LOG_TYPE_SCENARIO_STOP, (int)finishedStop);
    }
    driveController->setLastTime(clock->now());
}

/**
 * @brief   実行
*/
void DistanceRunScenario::Act(){

    initialize();

    for(;;){
        driveController->straightRun(power);

        //! ログを書き出しつつ、異常終了させない為に、適度な待ち時間が必要
        tslp_tsk(2);//今も必要？

        driveController->getDelta(&directionDelta, &distanceDelta);
        driveController->storageDeltaForSpeed(directionDelta, distanceDelta);
        driveController->outputSpeedLog();
    
        if(Stop()){
            return;
        }
    }
}

/**
 * @brief   停止
*/
bool DistanceRunScenario::Stop(){
    //! 走行体が指定距離走行したらストップ
    float   distanceTotal = driveController->getDistance(distanceDelta);
    bool isGreaterValue = isGreaterAbsoluteValue(distanceTotal, stopDistance);
    if (isGreaterValue && finishedStop) {
        driveController->stop();
    }    

    if (logger && (distanceDelta != 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        logger->addLogFloat(LOG_TYPE_DISTANCE_TOTAL, distanceTotal, isGreaterValue);
    }
    return  isGreaterValue;
}