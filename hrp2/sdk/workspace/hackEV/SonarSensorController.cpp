/**
 * @file    UltrasonicController.cpp
 * @brief   This file has UltrasonicController.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "SonarSensorController.h"


//! \addtogroup 超音波センサのポート
//@{
//! Sonar sensor    :   Port 3
const sensor_port_t sonar_sensor = EV3_PORT_3;

//@}


/**
 * @brief   超音波センサの初期化処理
 *
 * @return  なし
*/
void initialize_sonarsensor() {
    ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);

}

/**
 * @brief   超音波を一定間隔で発振する
 *
 * 
 *
 * @param   なし    
 * @return  なし
*/
void control_sonarsensor() {
    
}



