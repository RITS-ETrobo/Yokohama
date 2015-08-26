using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public abstract class State
	{
		/// <summary>機体</summary>
		protected EV3body _body;

		/// <summary>この状態の実行活動を繰り返すときの間隔[ms]</summary>
		public readonly int IntervalTime;

		public State(EV3body body, int intervalTime)
		{
			_body = body;
			IntervalTime = intervalTime;
		}

		/// <summary>任意のStateからこのStateに遷移するときに実行される処理</summary>
		public abstract void Enter();

		/// <summary>このStateにいる間, 繰り返し行われる処理</summary>
		public abstract void Do();

		/// <summary>このStateから任意のStateへ遷移するときの処理</summary>
		public abstract void Exit();

		/// <summary>遷移条件の判定</summary>
		/// <returns>発生したトリガー</returns>
		public abstract TriggerID JudgeTransition();

		#region とりあえず動かすために必要(後で超音波センサクラスに移される予定)

		///	超音波センサによる障害物検知距離 [mm]
		const int SONAR_ALERT_DISTANCE = 300;

		///	<summary>
		///	超音波センサによる障害物検知
		///	</summary>
		///	<returns>
		/// - true : 障害物有り
		/// - false : 障害物無し
		/// </returns>
		/// <param name="body">
		///	EV3bodyのインスタンス 
		/// </param>
		protected bool sonar_alert(EV3body body)
		{
			int distance = body.sonar.Read();
			if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0)){
				return true; /* 障害物を検知 */
			}else{
				return false; /* 障害物無し */
			}
		}

		#endregion

		#region とりあえず動かすために必要(後で尻尾モータクラスに移される予定)

		///	完全停止用モータ制御比例係数
		const float P_GAIN = 2.5F;

		///	完全停止用モータ制御PWM絶対最大値
		const int PWM_ABS_MAX = 60;

		///	完全停止時の角度[deg]
		protected const int TAIL_ANGLE_STAND_UP = 94;

		///	バランス走行時の角度[deg]
		protected const int TAIL_ANGLE_DRIVE = 3;

		///	<summary>
		///	走行体完全停止用モータの角度制御
		///	</summary>
		///	<returns>
		/// なし
		/// </returns>
		/// <param name="body">
		///	EV3bodyのインスタンス 
		/// </param>
		/// <param name="angle">
		///	モータ目標角度[度] 
		/// </param>
		protected void tail_control(EV3body body, int angle)
		{
			float pwm = (float)(angle - body.motorT.GetTachoCount ()) * P_GAIN; // 比例制御
			// PWM出力飽和処理
			if (pwm > PWM_ABS_MAX) {
				pwm = PWM_ABS_MAX;
			} else if (pwm < -PWM_ABS_MAX) {
				pwm = -PWM_ABS_MAX;
			}
			if ((sbyte)pwm == 0) {
				body.motorT.Brake();
			} else {
				body.motorT.SetPower((sbyte)pwm);
			}
		}

		#endregion

		#region とりあえず動かすために必要(後で光センサクラスに移される予定)

		///	白色の光センサ値
		protected const int LIGHT_BLACK = 0;

		///	黒色の光センサ値
		protected const int LIGHT_WHITE = 40;

		#endregion

		#region とりあえず動かすために必要(後でジャイロセンサクラスに移される予定)

		///	ジャイロセンサオフセット値
		protected const int GYRO_OFFSET = 0;

		#endregion
	}
}

