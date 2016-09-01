/**
 * @file    DriveController.cpp
 * @brief   走行を制御する
 */
#include <string.h>
#include <stdlib.h>

#include "instances.h"
#include "utilities.h"

#include "wheelSettings.h"
#include "logSettings.h"
#include "portSettings.h"

#include "pid_controller.h"

#include "DriveController.h"

//! class for driving
DriveController::DriveController()
    : motorWheelLeft(NULL)
    , motorWheelRight(NULL)
    , directionLast(0.0F)
    , directionTotal(0.0F)
    , distanceLast(0.0F)
    , distanceTotal(0.0F)
    , speedCalculator100ms(NULL)
    , speedCalculator1000ms(NULL)
    , lastGetDistanceLeft(0.0F)
    , lastGetDistanceRight(0.0F)
    , DURATION(100.0F)
    , KP(1.0F)
{
}

/**
 *  @brief  初期化処理
 *  @return true : 成功
 *  @return false : 失敗
*/
bool DriveController::initialize()
{
    ev3_speaker_play_tone(NOTE_E4, 100);

    directionLast = 0.0F;
    directionTotal = 0.0F;
    distanceLast = 0.0F;
    distanceTotal = 0.0F;

    lastGetDistanceLeft = 0.0F;
    lastGetDistanceRight = 0.0F;
    lastTime = 0;

    if (motorWheelLeft == NULL) {
        motorWheelLeft = new MotorWheel(EV3_MOTOR_LEFT);
    }

    if (motorWheelRight == NULL) {
        motorWheelRight = new MotorWheel(EV3_MOTOR_RIGHT);
    }

    if (speedCalculator100ms == NULL) {
        speedCalculator100ms = new SpeedCalculator(100);
    }

    if (speedCalculator1000ms == NULL) {
        speedCalculator1000ms = new SpeedCalculator(1000);
    }

    if (!motorWheelLeft || !motorWheelRight || !speedCalculator100ms || !speedCalculator1000ms) {
        return  false;
    }

    motorWheelLeft->initialize();
    motorWheelRight->initialize();
    speedCalculator100ms->initialize();
    speedCalculator1000ms->initialize();

    return  true;
}



/**
 * @brief   シナリオに従って走る
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void DriveController::run(scenario_running scenario)
{
    //! モーターの回転角、距離、方向を0に戻す
    initialize();

    if (logger) {
        logger->addLogFloat(LOG_TYPE_SCENARIO_DISTANCE, scenario.distance);
        logger->addLogInt(LOG_TYPE_SCENARIO_DIRECTION, scenario.direction);
        logger->addLogInt(LOG_TYPE_SCENARIO_POWER, scenario.power);
        logger->addLogInt(LOG_TYPE_SCENARIO_PATTERN, (int)scenario.pattern);
        logger->addLogInt(LOG_TYPE_SCENARIO_STOP, (int)scenario.stop);
    }
    
    lastTime = clock->now();
    
    //! ストップ監視しつつ、走行
    for (;;) {
        //! 走行
        if (runAsPattern(scenario)) {
            return;
        }

        //! ログを書き出しつつ、異常終了させない為に、適度な待ち時間が必要
        tslp_tsk(2);

        float   distanceDelta = 0.0F;
        float   directionDelta = 0.0F;
        getDelta(&directionDelta, &distanceDelta);
        if (stopByDistance(scenario, distanceDelta)) {
            return;
        }

        if (stopByDirection(scenario, directionDelta)) {
            return;
        }
    }
}

/**
 * @brief 	     モータを停止する
 * @param  brake ブレーキモードの指定，@a true （ブレーキモード）, @a false （フロートモード）
 * @retval E_OK  正常終了
 * @retval E_ID  不正のモータポート番号
 * @retval E_OBJ モータ未接続
 */
ER DriveController::stop(bool_t brake /*= true*/)
{
    ev3_speaker_play_tone(NOTE_E6, 100);

    ER  resultLeft = motorWheelLeft->stop(brake);
    ER  resultRight = motorWheelRight->stop(brake);

    if (resultLeft == resultRight) {
        return  resultRight;
    }

    if (resultLeft == E_OK) {
        return  resultRight;
    }

    writeFloatLCD(distanceTotal);
    writeFloatLCD(directionTotal);

    return  resultLeft;
}

/**
 * @brief   瞬間の走行体の向きと走行体中心の移動距離を取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  なし
*/
void DriveController::getDelta(float *directionDelta, float *distanceDelta)
{
    float   distanceDeltaLeft = motorWheelLeft->getDistanceDelta();
    float   distanceDeltaRight = motorWheelRight->getDistanceDelta();
    *directionDelta = ((distanceDeltaRight - distanceDeltaLeft) / EV3_TREAD) * 180 / Pi;
    *distanceDelta = (distanceDeltaRight + distanceDeltaLeft) / 2.0F;
}

/**
 * @brief   距離の補正比率を取得する
 * @param   targetDistance  目標距離[単位 : cm]
 * @param   resultDistance  各ホイールの移動実績[単位 : cm]
 * @return  correctedRatio 補正比率
*/
float DriveController::getCorrectedDistanceRatio(float targetDistance, float resultDistance){
    
    //! 目標との偏差
    float deviation = targetDistance - resultDistance;
    
    //! 差が低い方のホイールはpowerがint型で切り捨てられるため補正がかかる。
    //! 【要検討】一定水準以下の差のときは補正しないなどの処理が必要かもしれない。
    
    //! 補正比率※KPは比例制御係数：この係数によって、deviationの値によってどれだけpowerを補正するか調整可能
    //! 係数KPが1のときでは(power + 0.01*deviation)が補正されたpowerになる。ただしpowerはint型のため注意が必要。小数点以下は切り捨てられる
    float correctedRatio = KP * ((100 + deviation)/100);
    
    return correctedRatio;
}


/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  走行距離[単位 : cm]
*/
float DriveController::getDistance(float distanceDelta)
{
    distanceTotal += distanceDelta;

    if (logger && (distanceDelta != 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        //logger->addLogFloat(LOG_TYPE_DISTANCE, distanceDelta);

        logger->addLogFloat(LOG_TYPE_DISTANCE_TOTAL, distanceTotal);
    }

    return  distanceTotal;
}

/**
 * @brief   リセットしてからの走行体の向きを取得する
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @return  走行体の向き[単位 : 度]
*/
float DriveController::getDirection(float directionDelta)
{
    directionTotal += directionDelta;
    if (logger && (directionDelta != 0)) {
        logger->addLogFloat(LOG_TYPE_DIRECTION_TOTAL, directionTotal);
    }

    return  directionTotal;
}

/**
 * @brief   走行シナリオに従って走行する
 * @param   scenario    走行シナリオ
 * @return  true : 以降の処理を継続しない
 * @return  false : 以降の処理を継続する
*/
bool DriveController::runAsPattern(scenario_running scenario)
{
    switch (scenario.pattern) {
    case PINWHEEL:
        //! その場回転
        pinWheel(scenario.power);
        break;

    case SWITCH_SIDE_RIGHT:
    case SWITCH_SIDE_LEFT:
        //! ライントレースする縁を変更
        change_LineSide(scenario);
        return  true;

    case ONESIDE_PINWHEEL_RIGHT:
        //! 右回転(左タイヤのみ回転)
        motorWheelLeft->run(scenario.power);
        motorWheelRight->stop();
        break;

    case ONESIDE_PINWHEEL_LEFT:
        //! 左回転(右タイヤのみ回転)
        motorWheelRight->run(scenario.power);
        motorWheelLeft->stop();
        break;

    default:
        //! ライントレースせずに、直進走行する
        straightRun(scenario.power);
        break;
    }

    return  false;
}

/**
 * @brief   直進走行
 * @param   power   モーターへの入力
 * @return  なし
 */
void DriveController::straightRun(int power)
{
    float   directionDelta = 0.0F;
    float   distanceDelta = 0.0F;
    float   distanceDeltaRatio = 0.0F;
    int powerLeft = 0;
    int powerRight = 0;
    
    //! 最後に出力値として設定した値がなければ通常の出力値を代入
    if(lastPowerLeft == 0){
        lastPowerLeft = power;
    }
    if(lastPowerRight == 0){
        lastPowerRight = power;
    }
    
    SYSTIM currentTime = clock->now();
    if(currentTime - lastTime <= DURATION){
        //! DURATION以下なら前回の値そのまま
        powerLeft = lastPowerLeft;
        powerRight = lastPowerRight;
    }
    else{

        
        //! 左右のモーターの各トータル値の差の比率を取得
        //float distanceRatio = getCorrectDistanceRatio(distanceLeftTotal,distanceRightTotal);
        
        getCorrectedPower(power, &powerLeft, &powerRight);
        
        //! 最後の値の更新    
        lastTime = currentTime;
        lastPowerLeft = powerLeft;
        lastPowerRight = powerRight;
    }

    //! 実際の回転角度を見ながら左右の出力を調整
    motorWheelLeft->run(powerLeft);
    motorWheelRight->run(powerRight);
}

/**
 * @brief   その場回転
 * @return  なし
 */
void DriveController::pinWheel(int power)
{
    motorWheelLeft->run((-power));
    motorWheelRight->run(power);
}

/**
 * @brief   ラインの縁の変更処理
 * @param   scenario    走行シナリオ
 * @return  なし
 */
void DriveController::change_LineSide(scenario_running scenario)
{
    //! 最初に回転するタイヤ
    motor_port_t firstMoveWheel;

    //! 次に回転するタイヤ
    motor_port_t secondMoveWheel;
    
    switch (scenario.pattern) {
        case SWITCH_SIDE_LEFT:
            //! 移動したい縁が左のときは最初に右タイヤ、次に左タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_RIGHT;
            secondMoveWheel = EV3_MOTOR_LEFT;
            break;
            
        case SWITCH_SIDE_RIGHT:
            //! 移動したい縁が右のときは最初に左タイヤ、次に右タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_LEFT;
            secondMoveWheel = EV3_MOTOR_RIGHT;
            break;
            
        default:
            break;
    }
    
    //! ラインの黒線の上にいるフラグ（黒線をまたいで移動するため）
    bool onBlack = false;
    
    //! 片方だけのタイヤを反対側まで旋回
    ev3_motor_set_power(firstMoveWheel, scenario.power);
    float firstDirection = 0.0F;
    float   directionDelta = 0.0F;
    float   distanceDelta = 0.0F;
    for(;;){
        //! 動いた角度を記録
        driveController->getDelta(&directionDelta, &distanceDelta);
        firstDirection += directionDelta; 

        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
        if(colorValue < (black + 5)){
            onBlack = true;
        }
        
        if(colorValue > (white - 5) && onBlack){
            ev3_motor_stop(firstMoveWheel,true);
            writeFloatLCD((float)colorValue);
            break;
        }
    }
    
    //! モーターの回転角、距離、方向を0に戻す
    driveController->initialize();
    
    //! 目的の縁まで回転させたらその場回転で向きを戻す
    //ev3_motor_set_power(firstMoveWheel, (power/2));
    ev3_motor_set_power(secondMoveWheel, scenario.power);
    float secondDirection=0.0F;
    for(;;){
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        driveController->getDelta(&directionDelta, &distanceDelta);
        secondDirection += directionDelta; 

        //! 走行体が最初に動いた角度分戻ったらストップ
        if(abs(secondDirection) >= abs(firstDirection)){
            //writeFloatLCD((float)colorValue);
            stop();
            break;
        }
    } 
}

/**
 * @brief   指定した距離を走行していた場合、走行体を停止させる
 * @param   scenario    走行シナリオ
 * @param   distanceDelta   走行距離の増分
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDistance(scenario_running scenario, float distanceDelta)
{
    if (scenario.distance <= 0) {
        return  false;
    }

    //! 走行体が指定距離走行したらストップ
    float   distanceTotal = getDistance(distanceDelta);
    if (abs(distanceTotal) >= abs(scenario.distance)) {
        if (scenario.stop) {
            stop();
        }

        return  true;
    }    

    return  false;
}

/**
 * @brief   指定した角度だった場合、走行体を停止させる
 * @param   scenario    走行シナリオ
 * @param   directionDelta   角度の増分
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDirection(scenario_running scenario, float directionDelta)
{
    if (scenario.direction == -1) {
        return  false;
    }

    //! 走行体が指定した向きになったらストップ
    float   directionTotal = getDirection(directionDelta);
    if (abs(directionTotal) >= abs(scenario.direction)) {
        if(scenario.stop){
            stop();
        }

        return  true;
    }

    return  false;
}

/**
 * @brief   補正された出力を取得する
 * @param   power       モーターへの入力
 * @param   direction   角度[単位 : %]
 * @param   distanceRatio   左右のホイールの進んだ距離の比率
 * @param   powerLeft   左モーターへ与える入力
 * @param   powerRight  右モーターへ与える入力
 * @return  なし
 */
void DriveController::getCorrectedPower(int power, int *powerLeft, int *powerRight)
{   
    if (power == 0) {
        return;
    }
    
    //! DURATION以上時間が経過していた場合、左右のモーターの距離を比較して出力値を再設定する
    float distanceLeftTotal = motorWheelLeft->getDistance();
    float distanceRightTotal = motorWheelRight->getDistance();
    
    //! 左右のホイールの走行実績の平均値
    float averageDistanceTotal = (distanceLeftTotal + distanceRightTotal) / 2;
    
    //! 【要検討】getCorrectedRatioで偏差が0.01mm単位のものは無視するかどうかpower値はintなのでの差を許容するか
    float correctedRightRatio = getCorrectedDistanceRatio(averageDistanceTotal, distanceRightTotal);
    float correctedLeftRatio = getCorrectedDistanceRatio(averageDistanceTotal, distanceLeftTotal);
    
    //! 【要検討】指定した出力値、または前の出力値に補正をかけるか
    //! 左右のタイヤの出力値に補正を加算
    *powerRight = power * correctedRightRatio;
    *powerLeft = power * correctedLeftRatio;
    
    //! 補正したことをログに出力
    logger->addLog(LOG_NOTICE, "correct");
    
    //! 補正値をログに出力【ログが正しくでない】
    logger->addLogFloat(LOG_TYPE_CORRECTED_RATIO_RIGHT, (correctedRightRatio*100));
    logger->addLogFloat(LOG_TYPE_CORRECTED_RATIO_LEFT, (correctedLeftRatio*100));
    
    //! 補正された出力値をログに出力
    logger->addLogInt(LOG_TYPE_CORRECTED_POWER_RIGHT, *powerRight);
    logger->addLogInt(LOG_TYPE_CORRECTED_POWER_LEFT, *powerLeft);
}
