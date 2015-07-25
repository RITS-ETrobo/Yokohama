using System;
using System.Net.Sockets;

using MonoBrickFirmware.Movement;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.EV3
{
	struct EV3body{
		///	モータオブジェクト : 左
		public  Motor motorL;

		///	モータオブジェクト : 右
		public  Motor motorR;

		///	モータオブジェクト : ????
		public  Motor motorT;

		///	センサーオブジェクト : タッチセンサー
		public  EV3TouchSensor touch;

		///	センサーオブジェクト : 超音波センサー
		public  EV3UltrasonicSensor sonar;

		///	センサーオブジェクト : 色センサー
		public  EV3ColorSensor color;

		///	センサーオブジェクト : ジャイロセンサー
		public  EV3GyroSensor gyro;

		public static void init(ref EV3body body){
			body.motorL = new Motor (MotorPort.OutC);
			body.motorR = new Motor (MotorPort.OutB);
			body.motorT = new Motor (MotorPort.OutA);
			body.touch = new EV3TouchSensor (SensorPort.In1); 
			body.sonar = new EV3UltrasonicSensor (SensorPort.In2, UltraSonicMode.Centimeter); // return [mm]
			body.color = new EV3ColorSensor (SensorPort.In3, ColorMode.Reflection);
			body.gyro = new EV3GyroSensor (SensorPort.In4,	GyroMode.AngularVelocity);

			{
				sbyte turn = 0;
				body.motorLRCalibration (0, 0, 0, 0, ref turn);
			}
		}

		const int CALIBDATASIZE = 10;
		const int LEFTMOTOR = 0, RIGHTMOTOR = 1;
		private static int tL, tR;
		///	<summary>
		///	左右のモーターの個体差を打ち消す.
		/// 具体的には, 直前に設定したパワーと結果動いた角度から, これから動かす角度を設定する.
		/// また, この後Balcer.controlが呼ばれる前提である.
		///	</summary>
		///	<returns>
		/// 0: turn値修正不要
		/// 1: turn値を左曲がり気味に修正
		/// 2: turn値を右曲がり気味に修正
		/// -1: 異常終了
		/// </returns>
		/// <param name="pwmL">
		/// 前のループで左モーターに与えた力
		/// </param>
		/// <param name="pwmR">
		///	前のループで右モーターに与えた力
		/// </param>
		/// <param name="thetaL">
		/// 前のループで力を与えた結果, 動いた角度
		/// </param>
		/// <param name="thetaR">
		/// 前のループで力を与えた結果, 動いた角度
		/// </param>
		/// <param name="turn">
		/// in: 当初設定の進行方向
		/// out: キャリブ後の進行方向
		/// </param>
		public int motorLRCalibration(sbyte pwmL, sbyte pwmR, int thetaL, int thetaR, ref sbyte turn){
			// pwmL(PwmR)が設定されていなければキャリブ不要
			if (pwmL==0 && pwmR==0) {
				tL = thetaL;
				tR = thetaR;
				return 0;
			}

			double diffTL, diffTR;	// 左右モーターの差分
			double rateL, rateR;	// 左右モーターの, pwmからthetaへの変換率
			sbyte diffTurn;
			const double C_TURN = 1.0;	// turn変換係数

			diffTL = thetaL - tL;
			diffTR = thetaR - tR;

			rateL = (double)diffTL / pwmL;
			rateR = (double)diffTR / pwmR;

			diffTurn = (sbyte)((rateL-rateR) * C_TURN);

			turn += diffTurn;

			return (diffTurn>0)?1:(diffTurn<0)?2:0;
		}


//		const int CALIBDATASIZE = 10;
//		const int LEFTMOTOR = 0, RIGHTMOTOR = 1;
//		private static sbyte[,] power = new sbyte[2, CALIBDATASIZE];
//		private static int[,] theta = new int[2, CALIBDATASIZE];
//		private static int topCalibData = 0;
//		private static int nCalibData = 0;
//		///	<summary>
//		///	左右のモーターの個体差を打ち消す.
//		/// 具体的には, 直前に設定したパワーと結果動いた角度から, これから動かす角度を設定する.
//		/// また, この後Balcer.controlが呼ばれる前提である.
//		///	</summary>
//		///	<returns>
//		/// 0: turn値修正不要
//		/// 1: turn値を左曲がり気味に修正
//		/// 2: turn値を右曲がり気味に修正
//		/// -1: 異常終了
//		/// </returns>
//		/// <param name="pwmL">
//		/// 前のループで左モーターに与えた力
//		/// </param>
//		/// <param name="pwmR">
//		///	前のループで右モーターに与えた力
//		/// </param>
//		/// <param name="thetaL">
//		/// 前のループで力を与えた結果, 動いた角度
//		/// </param>
//		/// <param name="thetaR">
//		/// 前のループで力を与えた結果, 動いた角度
//		/// </param>
//		/// <param name="turn">
//		/// in: 当初設定の進行方向
//		/// out: キャリブ後の進行方向
//		/// </param>
//		public int motorLRCalibration(sbyte pwmL, sbyte pwmR, int thetaL, int thetaR, ref sbyte turn){
//			// pwmL(PwmR)が設定されていなければキャリブ不要
//			if (pwmL==0 && pwmR==0) {
//				resetCalibData ();
//				updateCalibData (pwmL, pwmR, thetaL, thetaR);
//				return 0;
//			}
//
//			double aveTL, aveTR;	// これまでに動いた左右モーター角度の平均
//			double diffTL, diffTR;	// 左右モーターの差分
//			double avePL, avePR;	// 左右モーターへ与えたパワーの平均
//			double rateL, rateR;	// 左右モーターの, pwmからthetaへの変換率
//			sbyte diffTurn;
//			const double C_TURN = 500;	// turn変換係数
//
//			getAverageTheta(out aveTL, out aveTR);
//			diffTL = thetaL - aveTL;
//			diffTR = thetaR - aveTR;
//
//			getAveragePower (out avePL, out avePR);
//			rateL = (double)diffTL / avePL;
//			rateR = (double)diffTR / avePR;
//
//			diffTurn = (sbyte)((rateL-rateR) * C_TURN);
//
//			turn += diffTurn;
//
//			// 更新処理
//			updateCalibData(pwmL, pwmR, thetaL, thetaR);
//
//			return (diffTurn>0)?1:(diffTurn<0)?2:0;
//		}
//
//		public void resetCalibData(){
//			for (int i = 0; i < CALIBDATASIZE; i++) {
//				power [LEFTMOTOR, topCalibData] = 0;
//				power [RIGHTMOTOR, topCalibData] = 0;
//				theta [LEFTMOTOR, topCalibData] = 0;
//				theta [RIGHTMOTOR, topCalibData] = 0;
//			}
//			topCalibData = 0;
//			nCalibData = 0;
//		}
//
//		public void updateCalibData(sbyte pl, sbyte pr, int tl, int tr){
//			topCalibData = (topCalibData==CALIBDATASIZE-1)? 0: topCalibData+1 ;
//
//			power [LEFTMOTOR, topCalibData] = pl;
//			power [RIGHTMOTOR, topCalibData] = pr;
//			theta [LEFTMOTOR, topCalibData] = tl;
//			theta [RIGHTMOTOR, topCalibData] = tr;
//
//			if (nCalibData < CALIBDATASIZE) {
//				nCalibData++;
//			}
//		}
//
//		const int LAG_POWER2THETA = 2;	// パワーを設定してから, それだけの速さで動き始めるまでのループ数
//
//		public void getAverageTheta(out double aveL, out double aveR ){
//			aveL = 0;
//			aveR = 0;
//			for (int i = 0; i < nCalibData; i++) {
//				if( (i>topCalibData && i<nCalibData) || i<topCalibData-LAG_POWER2THETA){
//					aveL += (double)theta [LEFTMOTOR, i] / (nCalibData-LAG_POWER2THETA);
//					aveR += (double)theta [RIGHTMOTOR, i] / (nCalibData-LAG_POWER2THETA);
//				}
//			}
//		}
//
//		public void getAveragePower(out double aveL, out double aveR ){
//			aveL = 0;
//			aveR = 0;
//			for (int i = 0; i < nCalibData; i++) {
//				if ((i > topCalibData && i < nCalibData) || i < topCalibData - LAG_POWER2THETA) {
//					aveL += theta [LEFTMOTOR, i] / (nCalibData-LAG_POWER2THETA);
//					aveR += theta [RIGHTMOTOR, i] / (nCalibData-LAG_POWER2THETA);
//				}
//			}
//		}
//
	}
}

