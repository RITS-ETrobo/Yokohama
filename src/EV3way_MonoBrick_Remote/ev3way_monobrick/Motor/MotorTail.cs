using MonoBrickFirmware.Movement;

using System.Threading;		// use WaitHandle

using MonoBrickFirmware.Display;

namespace ETRobocon.EV3
{

	public class MotorTail
	{

		public const int TAIL_ANGLE_STAND_UP = 96;   //完全停止時の角度[deg]
		public const int TAIL_ANGLE_DRIVE = 5;       //バランス走行時の角度[deg]
		public const int TAIL_ANGLE_LOOKUPGATE = 70; //ルックアップゲート用角度[deg]. 調整済み.
		private const int TAIL_ANGLE_MAX = 150;		 //尻尾の角度の最大. この角度を上回る制御はこの角度に収められる.
		private const int TAIL_ANGLE_MIN = 0;		 //尻尾の角度の最小. この角度を下回る制御はこの角度に収められる

		///	完全停止用モータ制御比例係数
		private const float P_GAIN = 2.5F;

		///	完全停止用モータ制御PWM絶対最大値
		private const int PWM_ABS_MAX = 60;

		/// <summary>制御する尻尾モーター</summary>
		private Motor motorTail;

		/// <summary>SetMotorAngleImmediately(もしくはSetMotorAngleSlowly)で指定したときの角度</summary>
		private int startAngle;

		/// <summary>最終的に一致させる角度</summary>
		private int targetAngle;

		/// <summary>段階的に動かすときに, 目標とする角度</summary>
		private int subTargetAngle;

		/// <summary>1段階で動かす角度</summary>
		private float subTargetAnglePerStep;

		/// <summary>targetAngleに持っていくまでの段階数</summary>
		private int totalSteps;

		/// <summary>現在の段階/totalSteps</summary>
		private int currentStep;

		/// <summary>Create instance</summary>
		public MotorTail ()
		{
			motorTail = new Motor (MotorPort.OutA);

			// 初回アクセスすることで, staticメンバの準備に時間がかかるのを防げる(らしい).
			motorTail.SetPower (0);
			motorTail.ResetTacho ();

			int count_resettailtacho = 100;
			while (count_resettailtacho > 0) {
				motorTail.SetPower (-5);
				count_resettailtacho--;
				Thread.Sleep (4);
			}
			motorTail.Brake ();
			motorTail.ResetTacho ();
		}

		/// <summary>Destroy instance</summary>
		~MotorTail(){
			TurnOff ();
		}

		///<summary>Turn the motor off.</summary>
		public void TurnOff()
		{
			motorTail.Off ();
		}

		/// <summary>
		/// 尻尾モーターの角度を設定する.
		/// </summary>
		/// <param name="angle">設定したい角度</param>
		public void SetMotorAngleImmediately(int angle)
		{
			if (angle > TAIL_ANGLE_MAX) {
				angle = TAIL_ANGLE_MAX;
			}
			if (angle < TAIL_ANGLE_MIN) {
				angle = TAIL_ANGLE_MIN;
			}
			startAngle = motorTail.GetTachoCount ();
			targetAngle = angle;
			totalSteps = 1;
			currentStep = 1;
			subTargetAnglePerStep = targetAngle - startAngle;
			subTargetAngle = targetAngle;
		}

		/// <summary>
		/// 尻尾モーターの角度を指定した段階数を踏んで設定する.
		/// </summary>
		/// <param name="angle">設定したい角度</param>
		/// <param name="steps">段階数. 値が大きいほどゆっくり設定する</param>
		public void SetMotorAngleSlowly(int angle, int steps)
		{
			if (angle > TAIL_ANGLE_MAX) {
				angle = TAIL_ANGLE_MAX;
			}
			if (angle < TAIL_ANGLE_MIN) {
				angle = TAIL_ANGLE_MIN;
			}
			startAngle = motorTail.GetTachoCount ();
			targetAngle = angle;
			if (steps < 1) {
				steps = 1;
			}
			totalSteps = steps;
			currentStep = 1;
			subTargetAnglePerStep = (float)(targetAngle - startAngle) / totalSteps;
			subTargetAngle = (int)(startAngle + subTargetAnglePerStep * currentStep);
		}


		/// <summary>
		/// メインループでの尻尾制御を行う.
		/// メインループに入る前に, SetMotorAngleImmediately(), もしくはSetMotorAngleSlowly()の
		/// どちらが呼ばれていなければならない. 
		/// </summary>
		public void UpdateTailAngle(){
			float pwm;
			int currentAngle;
			currentAngle = motorTail.GetTachoCount ();

			// 段階制御が終了していたら, 単純な比例制御
			if (currentStep > totalSteps) {
				pwm = (targetAngle - currentAngle)*P_GAIN;
			}else{
				// 尻尾が段階的目標角度になっていたら, 次の目標角度に更新
				if (IsReachedSubTargetAngle (currentAngle)) {
					currentStep++;
					subTargetAngle = (int)(startAngle + subTargetAnglePerStep * currentStep);
				}
				// モーターに送るパワーを決定する.
				pwm = (subTargetAngle - currentAngle) * P_GAIN; // 比例制御
			}
			// PWM出力飽和処理
			if (pwm > PWM_ABS_MAX) {
				pwm = PWM_ABS_MAX;
			} else if (pwm < -PWM_ABS_MAX) {
				pwm = -PWM_ABS_MAX;
			}

			// 尻尾のモーターに送る動力を設定. 
			if ((sbyte)pwm == 0) {
				motorTail.Brake();
			} else {
				motorTail.SetPower((sbyte)pwm);
			}
		}

		/// <summary>
		/// 尻尾が段階目標角度に到達したか判定する.
		/// 自重による負荷で追い越してしまう可能性があるので, このような条件式を採用している.
		/// </summary>
		/// <returns><c>true</c> 現在の尻尾角度が, 段階目標角度に到達している; それ以外は,<c>false</c>.</returns>
		/// <param name="currentAngle">現在の尻尾角度</param>
		private bool IsReachedSubTargetAngle(int currentAngle){
			return (startAngle - targetAngle) * (currentAngle - subTargetAngle) <= 0;
		}
	}
}

