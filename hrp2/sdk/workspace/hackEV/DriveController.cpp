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
    , initialized(false)
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
        //! 走行
        if (runAsPattern(scenario)) {
            return;
        }

        //! ログを書き出しつつ、異常終了させない為に、適度な待ち時間が必要
        tslp_tsk(2);

        SYSTIM  currentTime = clock->now();
        float   distanceDelta = 0.0F;
        float   directionDelta = 0.0F;
        getDelta(&directionDelta, &distanceDelta);

        DISTANCE_RECORD record;
        memset(&record, '\0', sizeof(DISTANCE_RECORD));
        record.currentTime = currentTime;
        record.distanceDelta = distanceDelta;
        record.directionDelta = directionDelta;
        speedCalculator100ms->add(record);

        DISTANCE_RECORD record_lap;
        float   averageSpeed = speedCalculator100ms->getSpeed(&record_lap);
        bool    needReturn = stopByDistance(scenario, distanceDelta) | stopByDirection(scenario, directionDelta);
        if (needReturn) {
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
        //! その場回転
        pinWheel(scenario.power, scenario.direction);
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

    default:
        //! ライントレースせずに、直進走行する
        straightRun(scenario.power);
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
    lastPowerLeft = scenario.power;
    lastPowerRight = scenario.power;

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
            writeFloatLCD((float)colorValue);
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
 * @param   distanceDelta   走行距離の増分
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDistance(scenario_running scenario, float distanceDelta)
{
    if (scenario.stopConditionPattern != DISTANCE_STOP) {
        return  false;
    }

    //! 走行体が指定距離走行したらストップ
    getDistance(distanceDelta);
    bool isGreaterValue = isGreaterAbsoluteValue(distanceScenario, scenario.distance);
    if (isGreaterValue && scenario.stop) {
        stop();
    }    

    if (logger && (distanceDelta != 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        //logger->addLogFloat(LOG_TYPE_DISTANCE, distanceDelta, true);
        logger->addLogFloat(LOG_TYPE_DISTANCE_TOTAL, distanceScenario, isGreaterValue);
    }

    return  isGreaterValue;
}

/**
 * @brief   指定した角度だった場合、走行体を停止させる(0度であれば判定しない)
 * @param   scenario    走行シナリオ
 * @param   directionDelta   角度の増分
 * @return  true : 停止可能
 * @return  false : 停止不可能
 */
bool DriveController::stopByDirection(scenario_running scenario, float directionDelta)
{
    if (scenario.stopConditionPattern != DIRECTION_STOP) {
        return  false;
    }

    //! 走行体が指定した向きになったらストップ
    getDirection(directionDelta);
    bool isGreaterValue = isGreaterAbsoluteValue(directionScenario, scenario.direction);
    if (isGreaterValue && scenario.stop){
        stop();
    }

    if (logger && (directionDelta != 0)) {
        logger->addLogFloat(LOG_TYPE_DIRECTION_TOTAL, directionScenario, isGreaterValue);
    }

    return  isGreaterValue;
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
 * @param   targetDirection  目標角度[°]
 * @param   power  基準のパワー値
 * @param   powerLeft  左モーターへ与える入力
 * @param   powerRight  右モーターへ与える入力
 * @return  なし
 */
void DriveController::getPowerForCurvatureRadius(enum runPattern pattern, float curvatureRadius, int power, int *powerLeft, int *powerRight){
    if (power == 0) {
        return;
    }

    //! 指定された曲率半径を指定のパワー値で進むための角速度を算出
    float targetDirectionRadian = power * speedPerOnePower / curvatureRadius;

    //! この左右のパワーの差があれば指定した角速度で曲がることができる
    //! 【TODO】speedPerOnePower定数ではなく、速度との変換をする?
    float adjustPowForCurve = targetDirectionRadian * EV3_TREAD / speedPerOnePower; 

    //! 一つのホイールへの調整量（パワー）
    int adjustPowForCurverToOneWheel = (int)abs(adjustPowForCurve / 2);

    //! カーブ方向によって調整するホイールを変更する(符号をそのまま加算すると減算調整が加算調整になることもあるため絶対値で調整)
    int sign = (pattern == NOTRACE_CURVE_LEFT) ? -1 : 1;
    *powerLeft = power + sign * adjustPowForCurverToOneWheel;
    *powerRight = power - sign * adjustPowForCurverToOneWheel;
    
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

            //! 動かない場合は角度移動はしない
            if (differenceX==0&& differenceY==0)
            {
                return 0;
            }
            if (differenceX == 0 && differenceY > 0)
            {
                return -startDirection;
            }
            if (differenceX == 0 && differenceY < 0)
            {
                return 180 - startDirection;
            }       
            if (differenceY == 0 && differenceX > 0)
            {
                return 90 - startDirection;
            }
            if (differenceY == 0 && differenceX < 0)
            {
                return -90 - startDirection;
            }

            //! ｘ座標を基準としたラジアン
            float Radian = atan2(differenceY, differenceX);

            float deg = to_deg(Radian);

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

            //! 現在の向きを考慮して、最終的に動く角度を算出
            float moveDirection = targetDirection - startDirection;

            return moveDirection;
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
void DriveController::jitteryMovementFromCoordinate(int power, float startX, float startY, float startDirection, float endX, float endY){

    
    //! 最初にその場で動く角度
    float moveDirection = directionFromCoordinateForJitteryMovement(startX, startY, startDirection, endX, endY);
    
    //! 目標の座標の向きまでその場回転
    scenario_running pinWheelScenario={30, 0.0F, moveDirection, PINWHEEL, true,0,DIRECTION_STOP};
    
    run(pinWheelScenario);

    //! 座標点間の直線距離
    float moveDistance = distanceFromCoordinateForJitteryMovement(startX, startY, endX, endY);
    
    //! 目標の座標まで直進
    scenario_running straghtScenario={30, moveDistance, 0.0F, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP};
    run(straghtScenario);
}

/**
 * @brief   座標指定移動のための変数を用意して実行させる
 * @param   目標座標が書かれたシナリオ
 * @return  
 */
void DriveController::manageMoveCoordinate(scenario_coordinate _coordinateScenario){
    //! [ 【TODO】現在の座標を取得【現在の位置が取得できるようになったら実装TODO】

    //! かくかく移動：スタート地点の座標と角度は0を「仮指定」（本来は現在の座標と向きを入れること）
    jitteryMovementFromCoordinate(_coordinateScenario.power, 0,0,directionTotal, _coordinateScenario.targetX, _coordinateScenario.targetY);
}

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

        *x = -(float)sqrt(pow(unit,2) / (1 + pow((float)tan(to_rad(deg)), 2)));
        *y = *x * (float)tan(to_rad(deg));
        return;
    }
    if (Direction < -180)
    {
        *x = (float)sqrt(pow(unit, 2) / (1 + pow((float)tan(to_rad(deg)), 2)));
        *y = *x * (float)tan(to_rad(deg));
        return;
    }

    if (Direction > 0 && Direction < 180)
    {
        *x = (float)sqrt(pow(unit, 2) / (1 + pow((float)tan(to_rad(deg)), 2)));
        *y = *x * (float)tan(to_rad(deg));
        return;
    }

    if (Direction > 180)
    {
        *x = -(float)sqrt(pow(unit, 2) / (1 + pow((float)tan(to_rad(deg)), 2)));
        *y = *x * (float)tan(to_rad(deg));
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