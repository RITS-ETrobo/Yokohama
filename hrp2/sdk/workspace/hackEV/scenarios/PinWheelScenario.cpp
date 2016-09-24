/**
 * @file    PinWheelScenario.cpp
 * @brief  その場回転のシナリオクラス
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
#include "PinWheelScenario.h"

//! 距離まで走るクラス
PinWheelScenario::PinWheelScenario(int power, float stopDirection, bool finishedStop)
    : power(power)
    , stopDirection(stopDirection)
    , finishedStop(finishedStop)
    , directionLast(0.0F)
    , distanceDelta(0.0F)
    , directionDelta(0.0F)
    
{
}

/**
 * @brief   初期化
*/
void PinWheelScenario::initialize(){
    driveController->initialize();
    driveController->setLastWheelPower((-1)*power, power);
    if (logger) {
        logger->addLog(LOG_NOTICE, "PinWheelScenario");
        logger->addLogInt(LOG_TYPE_SCENARIO_POWER, power);
        logger->addLogFloat(LOG_TYPE_SCENARIO_DIRECTION, stopDirection);
        logger->addLogInt(LOG_TYPE_SCENARIO_STOP, (int)finishedStop);
    }
    driveController->setLastTime(clock->now());
}

/**
 * @brief   実行
*/
void PinWheelScenario::Act(){

    initialize();

    for(;;){
        driveController->pinWheel(power, stopDirection);
        
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
bool PinWheelScenario::Stop(){
    //! 走行体が指定の向きに達したらストップ
    float   directionScenario = driveController->getDirection(directionDelta);
    bool isGreaterValue = isGreaterAbsoluteValue(directionScenario, stopDirection);
    if (isGreaterValue && finishedStop) {
        driveController->stop();
    }    

    if (logger && (distanceDelta != 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        logger->addLogFloat(LOG_TYPE_DIRECTION_TOTAL, directionScenario, isGreaterValue);
    }
    return  isGreaterValue;
}