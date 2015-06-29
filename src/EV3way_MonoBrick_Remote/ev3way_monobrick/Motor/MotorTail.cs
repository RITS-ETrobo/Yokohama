using System;
using MonoBrickFirmware.Movement;


namespace ETRobocon.EV3
{

	public class MotorTail
	{

		public const int TAIL_ANGLE_STAND_UP = 89;   //完全停止時の角度[deg]	//91から89に修正
		public const int TAIL_ANGLE_DRIVE = 5;       //バランス走行時の角度[deg]
		public const int TAIL_ANGLE_LOOKUPGATE = 42; //ルックアップゲート用角度. 調整済み.

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
			if (angle > TAIL_ANGLE_MAX) {
				angle = TAIL_ANGLE_MAX;
			}
			if (angle < TAIL_ANGLE_DRIVE) {
				angle = TAIL_ANGLE_DRIVE;
			}
			this.Angle = angle + 3;	// クラスで定めている角度からAPIが定めている角度に変換
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
		}
	}
}

