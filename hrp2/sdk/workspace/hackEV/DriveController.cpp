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

#include "DriveController.h"
#include "user_function.h"
#include "EV3Position.h"


//! class for driving
DriveController::DriveController()
    : motorWheelLeft(NULL)
    , motorWheelRight(NULL)
    , directionScenario(0.0F)
    , directionTotal(0.0F)
    , distanceScenario(0.0F)
    , distanceTotal(0.0F)
    , lastPowerLeft(0)
    , lastPowerRight(0)
    , lastTime(0)
    , DURATION(100.0F)
    , limitPower(55)
    , speedPerOnePower(0.84107F)
    , speedCalculator100ms(NULL)
    , positionTargetXLast(0.0F)
    , positionTargetYLast(0.0F)
    , initialized(false)
    , enabled(false)
    , lastColor(COLOR_NONE)
    , foundColor(false)
    , validColorTask(true)
    , colorHOSHITORI(COLOR_RED)
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

    if (initialized == false) {
        //  最初だけ初期化する
        directionTotal = 0.0F;
        distanceTotal = 0.0F;
        lastTime = 0;
        if (colorSensorController) {
            lastColor = colorSensorController->getColorID();
        }
    }

    //  シナリオごとに初期化する
    directionScenario = 0.0F;
    distanceScenario = 0.0F;

    if (motorWheelLeft == NULL) {
        motorWheelLeft = new MotorWheel(EV3_MOTOR_LEFT);
    }

    if (motorWheelRight == NULL) {
        motorWheelRight = new MotorWheel(EV3_MOTOR_RIGHT);
    }

    if (speedCalculator100ms == NULL) {
        speedCalculator100ms = new SpeedCalculator(100);
    }

    if (!motorWheelLeft || !motorWheelRight || !speedCalculator100ms) {
        return  false;
    }

    motorWheelLeft->initialize();
    motorWheelRight->initialize();
    speedCalculator100ms->initialize();

    initialized = true;
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
    initializeAsPattern(scenario);

    //! ストップ監視しつつ、走行
    for (;;) {
        bool    needReturn = stopByDistance(scenario) | stopByDirection(scenario) | stopByColor(scenario);
        if (needReturn) {
            return;
        }

        //! 走行
        if (runAsPattern(scenario)) {
            return;
        }
        //! ログを書き出しつつ、異常終了させない為に、適度な待ち時間が必要
        tslp_tsk(2);
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

    // writeFloatLCD(distanceTotal);
    // writeFloatLCD(directionTotal);

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
 * @brief   目標に対して補正として追加するパワー値を取得する
 * @param   targetDistance  目標の距離[単位 : cm]
 * @param   movedDistance  移動実績[単位 : cm]
 * @return  correctAddPower 補正で追加するPower値
*/
int DriveController::getCorrectedAddPower(float targetDistance, float movedDistance){
    
    //! 左右のホイールの距離の差
    float deviation = targetDistance - movedDistance;
    
    //! 左右の差が「1パワー分の100ms間の走行距離」ごとに、１パワーずつ補正をかける(分母を速度を100msごとになるよう1/10にすると分母が小さくなりすぎるため、分子を10倍)
    int correctAddPower = deviation / speedPerOnePower * 10;
    
    return correctAddPower;
}


/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  シナリオでの走行距離[単位 : cm]
*/
float DriveController::getDistance(float distanceDelta)
{
    distanceScenario += distanceDelta;
    distanceTotal += distanceDelta;
    return  distanceScenario;
}

/**
 * @brief   リセットしてからの走行体の向きを取得する
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @return  シナリオでの走行体の向き[単位 : 度]
*/
float DriveController::getDirection(float directionDelta)
{
    directionScenario += directionDelta;
    directionTotal += directionDelta;
    directionTotal = adjustValue(directionTotal, -180, 180);
    return  directionScenario;
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
        {
            bool softAcceleration = false;
            bool softDeceleration = false;

            //! 両モーターともに停止している場合は、加速処理を行うフラグを有効
            if(motorWheelRight->getCurrentPower() == 0 && motorWheelLeft->getCurrentPower() == 0){
                softAcceleration = true;
            }

            //! シナリオが停止させる条件を持っている場合は、減速処理を行うフラグを有効
            if(scenario.stop){
                softDeceleration = true;
            }

            //! 急発進、急停止しないためのパワー処理(角度はマイナスが入ることもあるので、絶対値の目標をとる)
            int softAccelPower = getSoftAccelAndDecelerationPower(scenario.power, fabsf(scenario.direction), fabsf(directionScenario), accelRangeForPinWheel, decelerationRangeForPinWheel, softAcceleration, softDeceleration);

            //! その場回転
            pinWheel(softAccelPower, scenario.direction);
        }
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
    

    case NOTRACE_CURVE_RIGHT:
    case NOTRACE_CURVE_LEFT:
        curveRun(scenario.pattern, scenario.power, scenario.curvatureRadius);
        break;
    
    case CORRECT_DIRECTION_BY_LINE:
    {

        //! 【TODO】向きをリセット。場所によってxかyどちらかをリセット
    }
        return true;

    default:
        {
            bool softAcceleration = false;
            bool softDeceleration = false;

            //! 両モーターともに停止している場合は、加速処理を行うフラグを有効
            if(motorWheelRight->getCurrentPower() == 0 && motorWheelLeft->getCurrentPower() == 0){
                softAcceleration = true;
            }

            //! シナリオが停止させる条件を持っている場合は、減速処理を行うフラグを有効
            if(scenario.stop){
                softDeceleration = true;
            }

            //! 急発進急加速しないためのパワーを取得
            int softAccelPower = scenario.power;
            if(scenario.power>0){
                softAccelPower = getSoftAccelAndDecelerationPower(scenario.power, scenario.distance, distanceScenario, accelRangeForStraight, decelerationRangeForStraight, softAcceleration, softDeceleration);
            }
            
            
            //! ライントレースせずに、直進走行する
            straightRun(softAccelPower);
        }
        break;
    }

    return  false;
}

/**
 * @brief   走行シナリオに従って、メンバ変数を初期化する
 * @param   scenario    走行シナリオ
 * @return  なし
*/
void DriveController::initializeAsPattern(scenario_running scenario)
{

    switch (scenario.pattern) {
    case PINWHEEL:
        //! その場回転
        lastPowerLeft = (-1) * scenario.power;
        break;

    case ONESIDE_PINWHEEL_RIGHT:
        //! 右回転(左タイヤのみ回転)
        lastPowerRight = 0;
        break;

    case ONESIDE_PINWHEEL_LEFT:
        //! 左回転(右タイヤのみ回転)
        lastPowerLeft = 0;
        break;

    default:
        //! ライントレースせずに、直進走行する

        break;
    }
}

/**
 * @brief   直進走行
 * @param   power   モーターへの入力
 * @return  なし
 */
void DriveController::straightRun(int power)
{
    int powerLeft = power;
    int powerRight = power;

    //! DURATION[ms]ごとに補正をかける
    SYSTIM currentTime = clock->now();
    if (currentTime - lastTime <= DURATION) {
        //! DURATION以下なら前回の値そのまま
        powerLeft = lastPowerLeft;
        powerRight = lastPowerRight;
    } else {
        //! 補正したパワー値を取得
        if(power>0){
            getCorrectedPower(power, &powerLeft, &powerRight);
        }
        

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
 * @param   power   モーターへの入力
 * @param   degree  回転する向き ※回転方向を決定するためだけに使う
 * @return  なし
 */
void DriveController::pinWheel(int power, float degree)
{
    int powerLeft = power;
    int powerRight = power;

    //! DURATION[ms]ごとに補正をかける
    SYSTIM currentTime = clock->now();
    if (currentTime - lastTime <= DURATION) {
        //! DURATION以下なら前回の値そのまま
        powerLeft = lastPowerLeft;
        powerRight = lastPowerRight;
    } else {
        //! 補正したパワー値を取得
        getCorrectedPower(power, &powerLeft, &powerRight);

        //! 最後の値の更新
        lastTime = currentTime;
        lastPowerLeft = powerLeft;
        lastPowerRight = powerRight;
    }
    
    //! 止まるときの角度がプラスであれば左周り
    int sign = (degree >= 0) ? -1 : 1;
    motorWheelLeft->run(sign *power);
    motorWheelRight->run(-1 * sign *power);
}

/**
 * @brief   ラインの縁の変更処理をおこなう。初期状態では、移動したい縁が左のときは最初に右タイヤ、次に左タイヤを旋回させて切り替えるものとする
 * @param   scenario    走行シナリオ
 * @return  なし
 */
void DriveController::change_LineSide(scenario_running scenario)
{
    //! 最初に回転するタイヤ
    motor_port_t firstMoveWheel = EV3_MOTOR_RIGHT;

    //! 次に回転するタイヤ
    motor_port_t secondMoveWheel = EV3_MOTOR_LEFT;

    if (scenario.pattern == SWITCH_SIDE_RIGHT) {
        //! 移動したい縁が右のときは最初に左タイヤ、次に右タイヤを旋回させて切り替え
        firstMoveWheel = EV3_MOTOR_LEFT;
        secondMoveWheel = EV3_MOTOR_RIGHT;
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
        getDelta(&directionDelta, &distanceDelta);
        firstDirection += directionDelta; 

        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
        if(colorValue < (black + 5)){
            onBlack = true;
        }
        
        if(colorValue > (white - 5) && onBlack){
            ev3_motor_stop(firstMoveWheel,true);
            // writeFloatLCD((float)colorValue);
            break;
        }
    }
    
    //! モーターの回転角、距離、方向を0に戻す
    initialize();
    
    //! 目的の縁まで回転させたらその場回転で向きを戻す
    //ev3_motor_set_power(firstMoveWheel, (power/2));
    ev3_motor_set_power(secondMoveWheel, scenario.power);
    float secondDirection=0.0F;
    for(;;){
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        getDelta(&directionDelta, &distanceDelta);
        secondDirection += directionDelta; 

        //! 走行体が最初に動いた角度分戻ったらストップ
        if(isGreaterAbsoluteValue(secondDirection, firstDirection)){
            //writeFloatLCD((float)colorValue);
            stop();
            break;
        }
    } 
}

/**
 * @brief   指定した距離を走行していた場合、走行体を停止させる
 * @param   scenario    走行シナリオ
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDistance(scenario_running scenario)
{
    if (scenario.stopConditionPattern != DISTANCE_STOP) {
        return  false;
    }

    //! 走行体が指定距離走行したらストップ
    bool isGreaterValue = isGreaterAbsoluteValue(distanceScenario, scenario.distance);
    if (isGreaterValue && scenario.stop) {
        stop();
    }    

    return  isGreaterValue;
}

/**
 * @brief   指定した角度だった場合、走行体を停止させる(0度であれば判定しない)
 * @param   scenario    走行シナリオ
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDirection(scenario_running scenario)
{
    if (scenario.stopConditionPattern != DIRECTION_STOP) {
        return  false;
    }

    //! 走行体が指定した向きになったらストップ
    bool isGreaterValue = isGreaterAbsoluteValue(directionScenario, scenario.direction);
    if (isGreaterValue && scenario.stop){
        stop();
    }

    return  isGreaterValue;
}

/**
 * @brief   指定した色だった場合、走行体を停止させる(0度であれば判定しない)
 * @param   scenario    走行シナリオ
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByColor(scenario_running scenario){
    
    //! ストップカラーが設定されていない場合は判定しない
    if(scenario.stopColorID == COLOR_NONE){
        return false;
    }

    return judgeStopColor(scenario.stopColorID);
}

/**
 * @brief   直線走行のための補正された出力を取得する
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
    
    //! 絶対値に変換し、左ホイールの実績距離を目標として、右モーターに補正として追加するパワー値を取得する
    int correctedAddRightPower = getCorrectedAddPower(fabsf(distanceLeftTotal), fabsf(distanceRightTotal));

    //! 右モーターに補正値の符号を調整して加算する
    *powerRight = addAdjustValue(power, correctedAddRightPower);
    *powerLeft = power;
    
    //! 補正したことをログに出力
    logger->addLog(LOG_NOTICE, "correct");
    
    //! 右モーターに追加する補正パワー値をログに出力
    logger->addLogInt(LOG_NOTICE, correctedAddRightPower);
    
    //! 補正された出力値をログに出力（その場回転はここで出力されたログ値とは違い、実際には一つがマイナスとなることに留意）
    logger->addLogInt(LOG_TYPE_CORRECTED_POWER_RIGHT, *powerRight);
    logger->addLogInt(LOG_TYPE_CORRECTED_POWER_LEFT, *powerLeft);
}

/**
 * @brief   "加算対象"の値が負の場合は、"加算値"の符号を逆にする
 * 理由：power補正値は後ろに進んでいるときも距離の絶対値を比べて計算した結果であるため、powerがマイナスの場合は補正値が反対になってしまうため
 * @param   targetValue  加算対象
 * @param   addvalue  加算する値
 * @return  計算結果
 */
int DriveController::addAdjustValue(int targetValue, int addvalue){
    int sumValue = targetValue;
    if(targetValue < 0){
        //! 加算対象が負の値の場合は加算値の符号を逆にする
        sumValue -= addvalue;
    }
    else{
        sumValue += addvalue;
    }
    return sumValue;
}

/**
 * @brief   曲率半径から左右のパワーを算出
 * 【参考】 http://www.ftech-net.co.jp/robot/howto/motion01.html
 * @param   targetDirection  目標角度[°]
 * @param   power  基準のパワー値
 * @param   powerLeft  左モーターへ与える入力
 * @param   powerRight  右モーターへ与える入力
 * @return  なし
 */
void DriveController::getPowerForCurvatureRadius(enum runPattern pattern, float curvatureRadius, int power, int *powerLeft, int *powerRight){
    if (power == 0) {
        *powerLeft = 0;
        *powerRight = 0;
        return;
    }

    //! 左右の速度比を算出
    float PowerRatioForCurve = (curvatureRadius - EV3_TREAD / (float)2)/(curvatureRadius + EV3_TREAD / (float)2);
    int powerWheelA = 2 * power / (1 + 1 / PowerRatioForCurve);
    int powerWheelB = 2 * power / (1 + PowerRatioForCurve);

    if (pattern == NOTRACE_CURVE_LEFT) {
        //! 左に曲がる場合
        *powerLeft = powerWheelA;
        *powerRight = powerWheelB;
    } else {
        //! 右に曲がる場合
        *powerLeft = powerWheelB;
        *powerRight = powerWheelA;
    }

    //! 【TODO】目標速度を算出して補正する必要もある
}


/**
 * @brief   曲線走行
 * ※注意：曲率半径7cm～351cmの範囲内であること。この範囲外を指定したときは一番近い範囲内の値に変更する。
 * @param   pattern 走行パターン（曲がる方向を判別する目的）
 * @param   power  基準のパワー値
 * @param   curvatureRadius  曲率半径[cm]
 * @return  
 */
void DriveController::curveRun(enum runPattern pattern, int power, float curvatureRadius){
    int powerLeft = power;
    int powerRight = power;

    //! 指定した曲率が許容範囲外であれば、一番近い範囲内の値に変更
    if (curvatureRadius < 7) {
        logger->addLog(LOG_NOTICE, "CurveEr");
        logger->addLog(LOG_NOTICE, "R=7");
        curvatureRadius = 7.0F;
    } else if (curvatureRadius > 351) {
        logger->addLog(LOG_NOTICE, "CurveEr");
        logger->addLog(LOG_NOTICE, "R=351");
        curvatureRadius = 351.0F;
    }
    
    //! パワー値が限界値を超えないようになるまでループ
    for(;;){
        //! カーブの曲率半径に適した左右のパワー値を取得
        getPowerForCurvatureRadius(pattern, curvatureRadius, power, &powerLeft, &powerRight);

        //! 調整後のパワー値が限界値を超えていたら曲率半径を優先するため、パワーと角速度を調整する
        if(powerRight > limitPower|| powerLeft > limitPower){
            //! 超えていたらパワーを下げて再度取得する
            power--;
            continue;
        }

        //! 調整後のパワー値がマイナスになっていないか確認
        if(powerRight < 0 || powerRight < 0){
            //! 超えていたらパワーを上げて再度取得する
            power++;
            continue;
        }

        break;
    }

    //! 変更したパワーをログ出力
    logger->addLogInt(LOG_TYPE_POWER_FOR_CURVE_LEFT, powerLeft);
    logger->addLogInt(LOG_TYPE_POWER_FOR_CURVE_RIGHT, powerRight);

    //! 実際の回転角度を見ながら左右の出力を調整
    motorWheelLeft->run(powerLeft);
    motorWheelRight->run(powerRight);
}

/**
 * @brief   座標点間の直線距離を算出
 * @param   startX 始点X
 * @param   startY 始点Y
 * @param   endX 終点X
 * @param   endY 終点Y
 * @return  
 */
float DriveController::distanceFromCoordinateForJitteryMovement(float startX, float startY, float endX, float endY){
    float differenceX = endX - startX;
    float differenceY = endY - startY;

    if (differenceX==0 && differenceY==0)
    {
        return 0;
    }

    if (differenceX !=0 && differenceY == 0)
    {
        return fabsf(differenceX);
    }

    if (differenceX == 0 && differenceY != 0)
    {
        return fabsf(differenceY);
    }

    //! ｘ座標を基準としたラジアン
    float Radian = atan2(differenceY, differenceX);

    //! 目標座標までの直線距離(sqrtを使うとimgビルドがエラーになるためコサインを使って求める)
    float moveDistance = fabsf(fabsf(differenceX) / cos(Radian));

    return moveDistance;
}

/**
 * @brief   座標点間を直線移動するための最初にその場で回転する角度を算出
 * @param   startX 始点X
 * @param   startY 始点Y
 * @param   endX 終点X
 * @param   endY 終点Y
 * @return  
 */
float DriveController::directionFromCoordinateForJitteryMovement(float startX, float startY, float startDirection, float endX, float endY){
    float differenceX = endX - startX;
    float differenceY = endY - startY;

    // //! 動かない場合は角度移動はしない
    if (differenceX == 0)
    {
        if (differenceY == 0)
        {
            return 0;
        }

        if (differenceY > 0)
        {
            return (-startDirection);
        }

        //! スタート時点の角度に最も近い方を選択(180と-180は同じ)
        if (startDirection > 0)
        {
            return (180 - startDirection);
        }

        if (startDirection < 0)
        {
            return (-180 - startDirection);
        }
    }

    if (differenceY == 0)
    {
        if (differenceX > 0)
        {
            return (90 - startDirection);
        }

        return (-90 - startDirection);
    }
    

    //! ｘ座標を基準としたラジアン
    float Radian = atan2(differenceY, differenceX);

    float deg = radian2degree(Radian);

    float targetDirection = 0;

    //! 目標座標の領域によって、Y軸を基準とした目標角度を調整する
    //! 左上領域
    if (differenceX > 0 && differenceY > 0)
    {
        targetDirection = 90 - deg;
    }
    //! 右上領域
    else if (differenceX < 0 && differenceY > 0)
    {
        targetDirection = 90 - deg;
    }
    //! 左下領域
    else if (differenceX > 0 && differenceY < 0)
    {
        targetDirection = 90 - deg;
    }
    //! 右下領域
    else
    {
        targetDirection = -270 - deg;
    }

    //! 回転量が最小となる回転角度を返す
    return shortestMoveDirection(targetDirection, startDirection);
}

/**
 * @brief   2つの角度で最小となる回転量を算出
 * @param   targetDirection  ターゲット角度
 * @param   startDirection 初期角度
 * @return  
 */
float DriveController::shortestMoveDirection(float targetDirection, float startDirection){
    //! ターゲットまでの角度を今のままか、360度反転させたときの移動量を比較（近い方を選択）
    if (fabsf((targetDirection - 360) - startDirection) < fabsf(targetDirection - startDirection))
    {
        //! スタート時の角度を今のままか、360度反転させたときの移動量を比較
        if (fabsf((targetDirection - 360) - (startDirection - 360)) < fabsf((targetDirection - 360) - startDirection))
        {
            //! スタート角度を３６０度反転とみなした方がいい場合はそちらを選択
            return (targetDirection - 360) - (startDirection - 360);
        }
        else
        {
            return (targetDirection - 360) - startDirection;
        }

    }
    else
    {
        //! スタート時の角度を今のままか、360度反転させたときの移動量を比較
        if (fabsf((targetDirection) - (startDirection - 360)) < fabsf(targetDirection - startDirection))
        {
            return targetDirection - (startDirection - 360);
        }
        else
        {
            return targetDirection - startDirection;
        }
    }
}

/**
 * @brief   座標指定移動(その場回転→直進で移動)（通称：かくかく移動版）
 * @param   power  走行パワー値
 * @param   startX 始点X
 * @param   startY 始点Y
 * @param   startDirection 始点での角度
 * @param   endX 終点X
 * @param   endY 終点Y
 * @param   endDirection 終点での角度
 * @return  
 */
void DriveController::jitteryMovementFromCoordinate(int power, float startX, float startY, float startDirection, float endX, float endY, uint8_t stopColorID /*= COLOR_NONE*/){

    
    //! 最初にその場で動く角度
    float moveDirection = directionFromCoordinateForJitteryMovement(startX, startY, startDirection, endX, endY);
    
    //! 目標の座標の向きまでその場回転
    scenario_running pinWheelScenario={power/2, 0.0F, moveDirection, PINWHEEL, true,0,DIRECTION_STOP, stopColorID};
    
    run(pinWheelScenario);

    //! 座標点間の直線距離
    float moveDistance = distanceFromCoordinateForJitteryMovement(startX, startY, endX, endY);
    
    //! 目標の座標まで直進
    scenario_running straghtScenario={power, moveDistance, 0.0F, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP, stopColorID};
    run(straghtScenario);
}

/**
 * @brief   座標指定移動のための変数を用意して実行させる
 * @param   目標座標が書かれたシナリオ
 * @return  
 */
void DriveController::manageMoveCoordinate(scenario_coordinate _coordinateScenario)
{
    EV3_POSITION currentPositionREAL;
    EV3_POSITION currentPositionMAP;
    float currentDirection = 0.0F;
    speedCalculator100ms->getPosition(&currentPositionREAL, &currentPositionMAP, &currentDirection);

    // writeFloatLCD(currentPositionREAL.x);
    // writeFloatLCD(currentPositionREAL.y);

    //! 滑らか走行
    //smoothMovementFromCoordinate(_coordinateScenario);

    //! マップ座標から現実座標へ変換
    float targetREAL_X = convertMapToREAL(_coordinateScenario.targetX);
    float targetREAL_Y = convertMapToREAL(_coordinateScenario.targetY);

    //! かくかく移動：スタート地点の座標と角度を「仮指定」（本来は現在の座標と向きを入れること）
    jitteryMovementFromCoordinate(_coordinateScenario.power, currentPositionREAL.x, currentPositionREAL.y, directionTotal, targetREAL_X, targetREAL_Y, _coordinateScenario.stopColorID);
}

#if FALSE
/**
 * @brief   滑らか座標移動
 * @param   目標座標が書かれたシナリオ
 * @return  
 */
void DriveController::smoothMovementFromCoordinate(scenario_coordinate _coordinateScenario){
    //! 滑らか走行（曲率半径を走行中に切り替えつつ移動する)
    //! 曲線を算出処理処理
    float a0=0;
	float a1=0;
	float a2=1;
	float a3=0;



    float x=-5; //! 初期のx座標を入れる


    //float distanceXaxis=x;
    initialize();

    //! モーターの回転角、距離、方向を0に戻す
    for(;;){
        float curvatureRadius = CalculationCurvatureRadius(a0,a1,a2,a3,x);
        
        curveRun(NOTRACE_CURVE_LEFT ,30, curvatureRadius);

        // float   distanceDelta = 0.0F;
        // float   directionDelta = 0.0F;
        // getDelta(&directionDelta, &distanceDelta);//updateでも更新されている

        // //! x軸に動いた距離 
        // distanceXaxis += distanceDelta*sin(degree2radian(directionDelta));
        
        //! 座標がx軸でプラス側に0.1cmずつ動いていると過程
        x+=0.0001;

        // //! 表示
        // writeFloatLCD(curvatureRadius);
        // writeFloatLCD(distanceDelta);
        // writeFloatLCD(directionDelta);

        if(x >= _coordinateScenario.targetX){
            stop();
            break;
        }

        //! 適度な待ち時間
        tslp_tsk(2);
    }
}
#endif  // FALSE

#if FALSE //モデル図記載の式から算出しようと思ったもの。うまくいかないので保留
/**
 * @brief   移動する曲線から瞬間の曲率半径を取得する
 * @param   目標座標が書かれたシナリオ
 * @return  
 */
float DriveController::getCurvatureRadius(float startX, float startY, float startDirection, float endX, float endY, float endDirection, float _s){

    float s = _s;

    float p0x=startX;
    float p0y=startY;

    float p1x=endX;
    float p1y=endY;
    
    //! startDirectionの大きさ１のベクトル成分を算出
    float v0x=0;
    float v0y=0;
    VectorFromDirection(startDirection, &v0x, &v0y);

    //! endDirectionの大きさ１のベクトル成分を算出
    float v1x=0;
    float v1y=0;
    VectorFromDirection(endDirection, &v1x, &v1y);
    
    float a1x = p1x;
    float a1y = p1y;

    float a2x = 3*p1x - 3*p0x - 2*v0x - v1x;
    float a2y = 3*p1y - 3*p0y - 2*v0y - v1y;

    float a3x = -2*p1x + 2*p0x + v0x + v1x;
    float a3y = -2*p1y + 2*p0y + v0y + v1y;

    float d1x=0;
    float d1y=0;
    getOnceDifferential(a1x, a1y, a2x, a2y, a3x, a3y, s, &d1x, &d1y);
    
    float d2x=0;
    float d2y=0;
    getSecondDifferential(a2x, a2y, a3x, a3y, s, &d2x, &d2y);

    //! 曲率半径Rの最終計算
    float R = pow(toVectorMagnitude(d1x, d1y),3) / multiplicationVector(d1x, d1y, d2x, d2y);

    return R;
}

/**
 * @brief   3次スプライン曲線の1回微分の値(ベクトル成分)
 * @return  
 */
void DriveController::getOnceDifferential(float a1x, float a1y, float a2x, float a2y, float a3x, float a3y,float s, float *d1x, float *d1y){
    //! 微分の計算結果
    *d1x = a1x + 2*a2x*s + 3*a3x*s*s;
    *d1y = a1y + 2*a2y*s + 3*a3y*s*s;
}

/**
 * @brief   3次スプライン曲線の2回微分の値(ベクトル成分)
 * @return  
 */
void DriveController::getSecondDifferential(float a2x, float a2y, float a3x, float a3y,float s, float *d2x, float *d2y){
    *d2x = 2*a2x + 6*a3x*s;
    *d2y = 2*a2y + 6*a3y*s;
}

/**
 * @brief   ベクトル成分から大きさを取得
 * @return  
 */
float DriveController::toVectorMagnitude(float x, float y){
    return sqrt(x*x+y*y);
}

/**
 * @brief   ベクトル積
 * @return  
 */
float DriveController::multiplicationVector(float x1, float y1, float x2, float y2){

    //! ベクトル積の計算が合っているか不安
    float a = x1*y2;
    float b = x2*y1;
    return toVectorMagnitude(a,b);
}

/**
 * @brief   角度をベクトル成分で表示する。ベクトルの大きさは１とする
 * @return  
 */
void DriveController::VectorFromDirection(float Direction, float *x, float *y){
    //!ベクトルの大きさを定義（仮で１とする）
    float unit = 1.0F;

    //Directionの範囲0～360と0～-360が来るものとする

    if (Direction == 0 || Direction == 360 || Direction == -360)
    {
        *x = 0;
        *y = unit;
        return;
    }

    if (Direction == 180 || Direction == -180)
    {
        *x = 0;
        *y = -unit;
        return;
    }

    if (Direction == 90 || Direction == -270)
    {
        *x = unit;
        *y = 0;
        return;
    }

    if (Direction == 270 || Direction == -90)
    {
        *x = -unit;
        *y = 0;
        return;
    }

    //! Tanなどの三角関数で扱うときの角度に変換
    float deg = degForTrigonometric(Direction);

    if (Direction < 0 && Direction > -180)
    {

        *x = -(float)sqrt(pow(unit,2) / (1 + pow((float)tan(degree2radian(deg)), 2)));
        *y = *x * (float)tan(degree2radian(deg));
        return;
    }
    if (Direction < -180)
    {
        *x = (float)sqrt(pow(unit, 2) / (1 + pow((float)tan(degree2radian(deg)), 2)));
        *y = *x * (float)tan(degree2radian(deg));
        return;
    }

    if (Direction > 0 && Direction < 180)
    {
        *x = (float)sqrt(pow(unit, 2) / (1 + pow((float)tan(degree2radian(deg)), 2)));
        *y = *x * (float)tan(degree2radian(deg));
        return;
    }

    if (Direction > 180)
    {
        *x = -(float)sqrt(pow(unit, 2) / (1 + pow((float)tan(degree2radian(deg)), 2)));
        *y = *x * (float)tan(degree2radian(deg));
        return;
    }
}

/**
 * @brief   三角関数で扱うときの角度（x軸プラスを基準とした角度）に変換
 * @return  
 */
float DriveController::degForTrigonometric(float direction){
    return -direction + 90;
}
#endif  // FALSE(モデル図記載の式から算出しようと思ったもの)

#if FALSE // 実装の時間が足りない為
/**
* @brief   曲率半径を計算する（モデル図のものではなく独自ver）
* 公式の参考サイト http://mathtrain.jp/curvature
* 公式：R=pow((1+pow(y´,2)),(3/2)) / y´´
* @return 現在のxの値に対する曲率半径
*/
float DriveController::CalculationCurvatureRadius(float a0, float a1, float a2, float a3, float x){

	float dy1 = 0;
	float dy2 = 0;

	//! 2次関数のdy1とdy2
	if(a3 == 0){
		dy1=OnceDifferentialOfQuadraticFunction(a1,a2,x);
		dy2=SecondDifferentialOfQuadraticFunction(a2);
	}
	

	//! 3次関数（＝xの3乗の項が存在する場合）
	if(a3 != 0){
		dy1 = OnceDifferentialOfCubicFunction(a1, a2, a3 ,x);
		dy2 = SecondDifferentialOfCubicFunction(a2, a3 ,x);
	}


	//! 分母が0の場合は曲率半径は無限大（つまり直進）
	if(dy2==0){
		//! 【TODO】充分に大きい値とする
		return 100000;
	}

	float R = pow((1+pow(dy1,2)),(3/2)) / dy2;
	return R;
}



/**
* @brief   ２次関数の1回微分の計算
* y´= 2*a2*x+a1
* @return  
*/
float DriveController::OnceDifferentialOfQuadraticFunction(float a1, float a2, float x){
	float dy1=2*a2*x+a1;
	return dy1; 
}

/**
* @brief   ２次関数の２回微分の計算
* y´´= 2*a2
* @return  
*/
float DriveController::SecondDifferentialOfQuadraticFunction(float a2){
	float dy2=2*a2;
	return dy2;
}


/**
* @brief   ３次関数の1回微分の計算
* y´= 2*a2*x+a1
* @return  
*/
float DriveController::OnceDifferentialOfCubicFunction(float a1, float a2, float a3, float x){
	float dy1 = 3*a3*x*x + 2*a2*x + a1;
	return dy1; 
}

/**
* @brief   ３次関数の２回微分の計算
* y´´= 2*a2
* @return  
*/
float DriveController::SecondDifferentialOfCubicFunction(float a2, float a3, float x){
	float dy2 = 6*a3*x + 2*a2;
	return dy2;
}
#endif  // FALSE // 実装の時間が足りない為

/**
 * @brief   走行体の位置を更新するタスク
 * @param   [in]    exinf   未使用
 * @return  なし
 */
void DriveController::updatePosition()
{
    if (!(speedCalculator100ms && motorWheelLeft && motorWheelRight)) {
        return;
    }

    SYSTIM  currentTime = clock->now();
    float   distanceDelta = 0.0F;
    float   directionDelta = 0.0F;
    getDelta(&directionDelta, &distanceDelta);
    getDistance(distanceDelta);
    getDirection(directionDelta);

    DISTANCE_RECORD record;
    memset(&record, '\0', sizeof(DISTANCE_RECORD));
    record.currentTime = currentTime;
    record.distanceDelta = distanceDelta;
    record.directionDelta = directionDelta;
    speedCalculator100ms->add(record);

    if (colorSensorController && validColorTask) {
        uint8_t currentColor = colorSensorController->getColorID(COLOR_MODE_COLOR);
        if (lastColor != currentColor) {
            lastColor = currentColor;
            if (logger) {
                //  前回と色が違った時は、ログに出力する
                logger->addLogInt(LOG_TYPE_COLOR, currentColor);
            }
        }
    }

    if (logger) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        //logger->addLogFloat(LOG_TYPE_DISTANCE, distanceDelta, true);
        logger->addLogFloat(LOG_TYPE_DISTANCE_SCENARIO, distanceScenario);
        logger->addLogFloat(LOG_TYPE_DISTANCE_TOTAL, distanceTotal);

        logger->addLogFloat(LOG_TYPE_DIRECTION_SCENARIO, directionScenario);
        logger->addLogFloat(LOG_TYPE_DIRECTION_TOTAL, directionTotal);
    }
}

/**
 * @brief   ラインを使って向きをリセットする
 * ホイールオドメトリで蓄積される誤差をラインを使ってリセットする
 * @param   
 * @param   
 * @return  成功、失敗
 */
bool DriveController::correctDirectionByLine(int power, orientationPattern findLineOrientation){

    //! 明度を使うので、通常のカラー取得は一度オフにする
    validColorTask = false;
    
    int moveCount = 10;

    //! ラインのどちら側にいるのか検討をつけて動かすほうのモーターを決める
    if(findLineOrientation == LEFT_PATTERN){
        //! ラインが走行体の左方向に位置する場合
        for(;;){
            //! 左のみ動かす
            motorWheelRight->run(power);
            motorWheelLeft->stop(true);

            moveCount++;
            for(;;){
                int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
                if(colorValue < (black + 5 +moveCount)){
                    stop();
                    break;
                }
            }

            //! 右のみ動かす
            motorWheelLeft->run(power);
            motorWheelRight->stop(true);

            for(;;){
                int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
                if(colorValue > (white - 5 -moveCount)){
                    stop();
                    break;
                }
            }

            //! 動かす距離が角ホイールごとにどちらも同じそして、動かす距離が十分に小さくなったらループを抜ける
            if(abs(white - 5 -moveCount) - abs(black + 5 +moveCount) <= 0){
                break;
            }
        }
    }
    else{
        //! ラインが走行体の右方向に位置する場合
        for(;;){
            //! 右のみ動かす
            motorWheelLeft->run(power);
            motorWheelRight->stop(true);

            moveCount++;
            for(;;){
                int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
                if(colorValue < (black + 5 +moveCount)){
                    stop();
                    break;
                }
            }

            //! 左のみ動かす
            motorWheelRight->run(power);
            motorWheelLeft->stop(true);

            for(;;){
                int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
                if(colorValue > (white - 5 -moveCount)){
                    stop();
                    break;
                }
            }

            //! 動かす距離が角ホイールごとにどちらも同じそして、動かす距離が十分に小さくなったらループを抜ける
            if(abs(white - 5 -moveCount) - abs(black + 5 +moveCount) <= 0){
                break;
            }
        }
    }

    //! 再び通常のカラーセンサーを有効
    validColorTask = true;
    return true;
}

/**
 * @brief   開始直後の急加速、目的距離で急停止しない処理(走行だけでなく回転でも使用する)
 * @param   [in]    power   シナリオで指定されたパワー
 * @param   [in]    stopValue   停止する値
 * @param   [in]    currentValue   現在の値
 * @param   [in]    accelerationRange   加速を行う範囲
 * @param   [in]    decelerationRange   減速を行う範囲
 * @param   [in]    softAcceleration   加速処理を行うフラグ
 * @param   [in]    softDeceleration   減速処理を行うフラグ
 * @return  なし
 */
int DriveController::getSoftAccelAndDecelerationPower(int power, float stopValue, float currentValue, float accelerationRange,float decelerationRange, bool softAcceleration, bool softDeceleration){

    //! 現在の値と停止値は絶対値で計算を行う
	currentValue = fabsf(currentValue);
    stopValue = fabsf(stopValue);

	int softAccelPower = power;

	if(stopValue - accelerationRange<0){
		//! 万が一、加速範囲よりも停止値が小さい場合は、加速範囲を停止距離の一定の割合で再定義
		// accelerationRange = stopValue*rangeRate;
	}else{
        softAccelPower = getAccelerationPower(accelStartPower, power, accelerationRange, currentValue, softAcceleration);
    }


	if(stopValue - decelerationRange<0){
		//! 万が一、減速範囲が停止距離を上回っていた場合は、減速範囲を停止距離の一定の割合で再定義
		//decelerationRange = stopValue*rangeRate;
	}else{
        softAccelPower = getDecelerationPower(decelerationFinishPower, softAccelPower, stopValue, decelerationRange, currentValue, softDeceleration);
    }

    

	return softAccelPower;
}

/**
 * @brief   減速処理のための、現在距離に対するパワー取得する
 * @param   [in]    finishPower   減速後最終のパワー
 * @param   [in]    runPower   走行しているときのパワー
 * @param   [in]    stopValue   ストップする値
 * @param   [in]    DecelerationRangeFromStopValue   ストップする箇所からどれだけ前を減速範囲とするか
 * @param   [in]    currentValue   現在の値
 * @return  なし
 */
int DriveController::getDecelerationPower(int finishPower,  int runPower, float stopValue, float DecelerationRangeFromStopValue, float currentValue, bool softDeceleration){

    if(softDeceleration == false){
        return runPower;
    }

	float beginDecelerationValue = stopValue-DecelerationRangeFromStopValue;

	//! 現在の距離が減速開始以下であればまだ減速しない
	if(currentValue < beginDecelerationValue){
		return runPower;
	}

	float slop = (runPower-finishPower)/(pow(beginDecelerationValue,2)-pow(stopValue,2));
	float Intercept = runPower - slop*pow(beginDecelerationValue,2);
	float decelerationPower = slop*pow(currentValue,2) + Intercept;


	//! もし最小値よりも小さくなってしまった場合は、最小値にする。
	if(decelerationPower<finishPower){
		return finishPower;
	}

	return decelerationPower;
}

/**
 * @brief   加速処理のための、現在距離に対するパワー取得する
 * @param   [in]    startPower   加速始めのパワー
 * @param   [in]    runPower   走行しているときのパワー
 * @param   [in]    accelerationRange   加速範囲（どのくらいで指定したrunPowerに到達するか）
 * @param   [in]    currentValue   現在の値
 * @return  なし
 */
int DriveController::getAccelerationPower(int startPower, int runPower, float accelerationRange, float currentValue,bool softAcceleration){
	
    //! 加速処理を行うフラグがなければ加速パワー調整を行わない
    // if(softAcceleration == false){
    //     return runPower;
    // }

	//! 現在の距離が加速距離を進んだあとであればもう加速しない
	if(currentValue > accelerationRange){
		return runPower;
	}
	
	float slop=(runPower-startPower)/pow(accelerationRange,2);
	float Intercept=startPower;
	float accelerationPower=slop*pow(currentValue,2) + Intercept;

	//! もし指定された最大パワーよりも大きくなってしまったときは最大値にする
	if(accelerationPower > runPower){
		return runPower;
	}

	return accelerationPower;
}

/**
 *  @param  ログを出力するかどうかを切り替える
 *  @return なし
*/
void DriveController::setEnabled(bool _enabled /*= true*/)
{
    enabled = _enabled;
}

/**
 *  @param  ログを出力するかどうかを返す
 *  @return true : ログを出力する false : ログを出力しない
*/
bool DriveController::isEnabled()
{
    return  enabled;
}

/**
 *  @param ラインを掴んで、向きを揃える一連の動作をまとめる
 *  @return なし
*/
orientationPattern DriveController::catchLineAndCorrectDirection(int power, float searchWidth, float searchHeight){
    enum orientationPattern findLineOrientation = catchLine(power, searchWidth, searchHeight);
    
    if(findLineOrientation == NONE_PATTERN){
        return NONE_PATTERN;
    }

    if(correctDirectionByLine(power, findLineOrientation)){
        return findLineOrientation;
    }

    return NONE_PATTERN;
}

/**
 *  @param ラインを掴む
 *  @return なし
*/
orientationPattern DriveController::catchLine(int power, float serchWidth, float searchHeight){

    enum orientationPattern findLineOrientation = LEFT_PATTERN;

    //! ラインを探す前の向きを覚えておく
    float beforeDirection = directionTotal;


    //! 左方向を探す
    jitteryMovementFromCoordinate(power, 0 , 0 , 0, serchWidth/2, searchHeight, COLOR_BLACK);

    //! 左方向探索で指定したカラー値を見つけていた
    if(foundColor){
        //! 一番最初の向き(beforeDirection)に直す
        rotateAbsolutelyDirection(power/2, beforeDirection);

        foundColor =false;//フラグを元に戻す
        return LEFT_PATTERN;
    }

    //! 左方向を探し終えるまでに向いた向き
    float leftMovedDirection =shortestMoveDirection(directionTotal, beforeDirection);


    //! 右方向を探す
    jitteryMovementFromCoordinate(power, 0 , 0 , leftMovedDirection, -serchWidth, searchHeight, COLOR_BLACK);

    //! 右方向探索で指定したカラー値を見つけていた
    if(foundColor){
        //! 一番最初の向き(beforeDirection)に直す
        rotateAbsolutelyDirection(power/2, beforeDirection);

        foundColor =false;//フラグを元に戻す
        return RIGHT_PATTERN;
    }

    

    //! 途中で黒線がある通知が来ればストップさせる



    //! 一番最初の向き(beforeDirection)に直す
    rotateAbsolutelyDirection(power/2, beforeDirection);

    //! 見つけた方向を返す
    return NONE_PATTERN;
}

/**
 *  @param 向きを絶対指定する（相対指定ではなく）
 * @param   [in]    power   回転パワー
 * @param   [in]    AbsolutelyTargetDirection   あわせたい向きの指定（絶対指定）
 *  @return なし
*/
void DriveController::rotateAbsolutelyDirection(int power, float AbsolutelyTargetDirection){
    
    //! ターゲット角度までの回転量
    float moveDirection = shortestMoveDirection(AbsolutelyTargetDirection, directionTotal);
    
    //! 目標の座標の向きまでその場回転
    scenario_running pinWheelScenario={power, 0.0F, moveDirection, PINWHEEL, true,0,DIRECTION_STOP};
    
    run(pinWheelScenario);
}

/**
 * @brief   走行をストップさせるカラー判定
 * @param   [in]    exinf   未使用
 * @return  ストップカラーだった場合はtrue、そうでなかった場合にはfalse
 */
bool DriveController::judgeStopColor(uint8_t stopColor){

    //! 停止色と同じだった場合は停止する
    if(lastColor == stopColor){
        foundColor = true;
        return true;
    }

    return false;
}

/**
 * @brief   x座標を設定 [pixcel値]
 * @param   [in]    exinf   未使用
 * @return  なし
 */
void DriveController::setNewPositionX(float newX){
    EV3_POSITION position;
    position.x = newX;
    speedCalculator100ms->setPosition(&position , 0.0F, EV3Position::CORRECT_POSITION_MAP_X);
}

/**
 * @brief   y座標を設定 [pixcel値]
 * @param   [in]    exinf   未使用
 * @return  なし
 */
void DriveController::setNewPositionY(float newY){
    EV3_POSITION position;
    position.y = newY;
    speedCalculator100ms->setPosition(&position , 0.0F, EV3Position::CORRECT_POSITION_MAP_Y);
}

/**
 * @brief   新しい向きを設定
 * @param   [in]    exinf   未使用
 * @return  なし
 */
void DriveController::setNewDirection(float newDirection){
    directionTotal = newDirection;
    EV3_POSITION position;
    speedCalculator100ms->setPosition(&position, newDirection, EV3Position::CORRECT_DIRECTION);
}

/**
 * @brief   マップ座標を現実座標に変換
 * @param   [in]    exinf   未使用
 * @return  なし
 */
float DriveController::convertMapToREAL(float map){
    return map * 2 / (float)7;
}

/**
 *  @brief  走行体の現在位置を設定する
 *  @param  position    座標
 *  @param  direction_  向き[単位 : 度]
 *  @param  updateType  更新する項目を指定する
*/
void DriveController::setPosition(EV3_POSITION *position, float direction_, uint8_t updateType /*= 0*/)
{
    if (speedCalculator100ms == NULL) {
        return;
    }

    speedCalculator100ms->setPosition(position, direction_, updateType);
}

/**
 *  @param  星とり探し
 *  @return 星とりを探しあてればtrue,探せなければfalse
*/
bool DriveController::searchHOSHITORI(int power, float searchWidth){

    int moveCount =0;
    for(;;){

        float firstDirection = directionTotal;

        //! 片側ずつ探してしらみつぶしで見つける
        motorWheelRight->run(power);
        motorWheelLeft->stop(true);

        moveCount++;
        for(;;){     
            if(isAnyHOSHITORIcolor()){
                stop();
                return true;
            }

            if(fabsf(directionTotal) > fabsf(searchWidth)){
                break;
            }
        }

        //! 右のみ動かす
        motorWheelLeft->run(power);
        motorWheelRight->stop(true);

        for(;;){     
            if(isAnyHOSHITORIcolor()){
                stop();
                return true;
            }

            if(fabsf(directionTotal) < fabsf(searchWidth)){
                break;
            }
        }

        //! 5回移動しても見つからなかった場合
        if(moveCount > 5){
            return false;
        }
    }
}

/**
 *  @param  星とりのどれかの色が真下にある
 *  @return あればtrue,なければfalse
*/
bool DriveController::isAnyHOSHITORIcolor(){
    if(lastColor == COLOR_RED || lastColor == COLOR_YELLOW || lastColor == COLOR_GREEN || lastColor == COLOR_BLUE){
        
        //! 星とりの色としてセット
        colorHOSHITORI = lastColor;
        return true;
    }
}

/**
 *  @param ラインを掴む
 *  @return なし
*/
orientationPattern DriveController::catchLineRIGHT(int power, float serchWidth, float searchHeight){

    enum orientationPattern findLineOrientation = LEFT_PATTERN;

    //! ラインを探す前の向きを覚えておく
    float beforeDirection = directionTotal;

    //! 右方向を探す
    jitteryMovementFromCoordinate(power, 0 , 0 , 0, -serchWidth/2, searchHeight, COLOR_BLACK);

    //! 右方向探索で指定したカラー値を見つけていた
    if(foundColor){
        //! 一番最初の向き(beforeDirection)に直す
        rotateAbsolutelyDirection(power/2, beforeDirection);

        foundColor =false;//フラグを元に戻す
        return RIGHT_PATTERN;
    }

    //! 左方向を探し終えるまでに向いた向き
    float leftMovedDirection =shortestMoveDirection(directionTotal, beforeDirection);

    //! 左方向を探す
    jitteryMovementFromCoordinate(power, 0 , 0 , leftMovedDirection, serchWidth, searchHeight, COLOR_BLACK);

    //! 左方向探索で指定したカラー値を見つけていた
    if(foundColor){
        //! 一番最初の向き(beforeDirection)に直す
        rotateAbsolutelyDirection(power/2, beforeDirection);

        foundColor =false;//フラグを元に戻す
        return LEFT_PATTERN;
    }

    


    

    

    //! 途中で黒線がある通知が来ればストップさせる



    //! 一番最初の向き(beforeDirection)に直す
    rotateAbsolutelyDirection(power/2, beforeDirection);

    //! 見つけた方向を返す
    return NONE_PATTERN;
}

//! カーブのシナリオを作成して走行させる
void DriveController::curveOfscenario(int power, float moveDirection, float curveRadius, runPattern curveOrientation){
    scenario_running curveScenario={power, 0.0F, moveDirection, curveOrientation, true, curveRadius , DIRECTION_STOP};
    
    run(curveScenario);
}