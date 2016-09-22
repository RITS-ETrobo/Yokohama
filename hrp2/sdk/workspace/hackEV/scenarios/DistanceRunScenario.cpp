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
DistanceRunScenario::DistanceRunScenario(int power, float stopDistance, bool runStop)
    : power(power)
    , stopDistance(stopDistance)
    , runStop(runStop);
    , directionLast(0.0F)
    , distanceDelta(0.0F)
    , directionDelta(0.0F)
    
{
}

/**
 * @brief   初期化
*/
void DistanceRunScenario::initialize(){
}

/**
 * @brief   実行
*/
void DistanceRunScenario::Act(){
    for(;;){
        driveController->straightRun(power);
        driveController->getDelta(&directionDelta, &distanceDelta);
        driveController->storageDeltaForSpeed();
        driveController->outputSpeedLog();
    
        if(Stop){
            return;
        }
    }
}

/**
 * @brief   停止
*/
bool DistanceRunScenario::Stop(){
    //! 走行体が指定距離走行したらストップ
    float   distanceTotal = getDistance(distanceDelta);
    bool isGreaterValue = isGreaterAbsoluteValue(distanceTotal, stopDistance);
    if (isGreaterValue && runStop) {
        driveController->stop();
    }    

    if (logger && (distanceDelta != 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        logger->addLogFloat(LOG_TYPE_DISTANCE_TOTAL, distanceTotal, isGreaterValue);
    }
    return  isGreaterValue;
}