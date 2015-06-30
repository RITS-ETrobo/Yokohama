using System;
using MonoBrickFirmware.Movement;

using System.Threading;		// use WaitHandle

namespace ETRobocon.EV3
{

	public class MotorTail
	{

		public const int TAIL_ANGLE_STAND_UP = 91;   //完全停止時の角度[deg]
		public const int TAIL_ANGLE_DRIVE = 5;       //バランス走行時の角度[deg]
		public const int TAIL_ANGLE_LOOKUPGATE = 42; //ルックアップゲート用角度. 調整済み.
		private const int TAIL_ANGLE_MAX = 150;		 //尻尾の角度の最大. この角度を超えた制御はこの角度に収められる.
		private const int TAIL_ANGLE_MIN = 0;		 //尻尾の角度の最小. この角度を下回る制御はこの角度に収められる

		///	完全停止用モータ制御比例係数
		private const float P_GAIN = 2.5F;

		///	完全停止用モータ制御PWM絶対最大値
		private const int PWM_ABS_MAX = 60;

		/// <summary>制御する尻尾モーター</summary>
		private Motor motorTail;

		/// <summary>尻尾モーターの, 上がりきった位置からの角度[deg]. </summary>
		public int Angle { private set; get; }

		/// <summary>Create instance</summary>
		public MotorTail ()
		{
			this.motorTail = new Motor (MotorPort.OutA);

			// 初回アクセスすることで, staticメンバの準備に時間がかかるのを防げる(らしい).
			this.motorTail.SetPower (0);
			this.motorTail.ResetTacho ();
		}


		///<summary>Turn the motor off.</summary>
		public void TurnOff()
		{
			this.motorTail.Off ();
		}

		/// <summary>
		/// 尻尾モーターの角度を設定する.
		/// </summary>
		/// <param name="angle">設定したい角度</param>
		public void SetMotorAngle(int angle)
		{
			WaitHandle motorWaitHandle;
			Int32 startTachoCount;
			Int32 moveTachoCount;
			sbyte speed;

			if (angle > TAIL_ANGLE_MAX) {
				angle = TAIL_ANGLE_MAX;
			}
			if (angle < TAIL_ANGLE_MIN) {
				angle = TAIL_ANGLE_MIN;
			}
			Angle = angle + 3;	// クラスで定めている角度からAPIが定めている角度に変換

			startTachoCount = motorTail.GetTachoCount ();
			moveTachoCount = Angle - startTachoCount;

			if (moveTachoCount > 0) {
				speed = 100;
			} else {
				speed = -100;
				moveTachoCount = 0 - moveTachoCount;
			}

			if (moveTachoCount == 0) {
				return;
			}

			/*motorWaitHandle = */motorTail.SpeedProfile (speed, 0, (uint)moveTachoCount, 0, true);
			//motorWaitHandle.WaitOne();

			/* これまでの尻尾制御ソースコード
			 * ループ(4ms周期?)の中で常に呼び出し, その呼び出しのときの回転数(角度)から計算したパワーを
			 * モーターに送ることで制御している.
			
			float pwm = (float)(Angle - this.motorTail.GetTachoCount ()) * P_GAIN; // 比例制御
			// PWM出力飽和処理
			if (pwm > PWM_ABS_MAX) {
				pwm = PWM_ABS_MAX;
			} else if (pwm < -PWM_ABS_MAX) {
				pwm = -PWM_ABS_MAX;
			}
			if ((sbyte)pwm == 0) {
				this.motorTail.Brake();
			} else {
				this.motorTail.SetPower((sbyte)pwm);
			}
			*/
		}
	}
}

