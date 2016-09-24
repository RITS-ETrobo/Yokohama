/**
 * @file    ScenarioManager.cpp
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
#include "ScenarioManager.h"

//! シナリオの管理・実行者
ScenarioManager::ScenarioManager()
{
}

/**
 * @brief   初期化
*/
void ScenarioManager::initialize()
{
    ScenarioList.clear();

    //! シナリオを設定
    ScenarioList.push_back(new DistanceRunScenario(30, 20.0F, true));
    ScenarioList.push_back(new PinWheelScenario(30, 180.0F, true));
}

/**
 * @brief   シナリオを実行
*/
void ScenarioManager::act()
{
    vector<Scenario*>::iterator begin,end;

    //! シナリオを一つずつ取り出して実行
    begin = ScenarioList.begin();
    end = ScenarioList.end();
    for(;begin != end; ++begin){
        (*begin)->Act();
  }
  //! 【TODO】メモリの開放を行うこと

}
