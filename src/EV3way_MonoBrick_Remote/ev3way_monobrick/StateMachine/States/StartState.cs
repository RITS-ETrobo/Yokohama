using System;
using System.Threading;
using ETRobocon.EV3;
using ETRobocon.Utils;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.StateMachine
{
	/// <summary>
	/// 直立停止から, 走行までの間の状態.
	/// 安定した走行を開始するための制御を行う.
	/// </summary>
	public class StartState : State
	{
		// MainClassのrun()より移してきた
		private int _batteryLevel;

		private const int START_COUNT = 5;	// 安定処理するまでのループ数
		private int counter;	// ループ回数カウンター. 既定値から0まで下がる.

		/// <summary>
		/// SrartStateのメンバを初期化する.
		/// </summary>
		/// <param name="body">Body.</param>
		public StartState(EV3body body) : base(body, 2)
		{
			counter = START_COUNT;
		}

		/// <summary>
		/// 走行開始前の初期化を行う.
		/// また, 尻尾を, 安定した走行開始を行う角度に指定する.
		/// </summary>
		public override void Enter()
		{
			var dialogRun = new InfoDialog("Running", false);
			dialogRun.Show();

			LogTask.LogRemote("EV3 run.");

			// 走行開始前にタイヤが動いていると自己位置推定に誤差が出てくるのでTachoCountの値をリセットする]
			//  - ココじゃなくてStateMachine.cs内の走行準備からの遷移であるnew Transition(StateID.Straight1, Nop)のNopのところでやるべき？
			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			// スタート時の尻尾角度に制御指定
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_START);
		}

		/// <summary>
		/// ループ数をカウントしながら尻尾を制御する.
		/// 同時に, 倒立制御を行う.
		/// </summary>
		public override void Do()
		{
			// スタート時のためのバランス制御
			_body.motorTail.UpdateTailAngle ();

			// 倒立制御
			sbyte forward;
			sbyte turn;
			int gyroNow = _body.gyro.Read();
			int thetaL = _body.motorL.GetTachoCount();
			int theTaR = _body.motorR.GetTachoCount();
			sbyte pwmL, pwmR;

			// ライトレース, ソナー検知処理はしない.
			forward = 20;
			turn = 0;

			Balancer.control (
				(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)theTaR, (float)_batteryLevel,
				out pwmL, out pwmR
			);

			if (pwmL == 0) {
				_body.motorL.Brake();
			} else {
				_body.motorL.SetPower(pwmL);
			}
			if (pwmR == 0) {
				_body.motorR.Brake();
			} else {
				_body.motorR.SetPower(pwmR);
			}

			// ループ数カウント
			counter--;
		}

		/// <summary>ステート終了処理は特になし</summary>
		public override void Exit()
		{
		}

		/// <summary>
		/// 遷移条件の判定
		/// 　規定ループ数に到達 -> TimeExpireトリガー
		/// 　stopコマンド受信 -> StopCommandトリガー
		/// </summary>
		/// <returns>発生したトリガー</returns>
		public override TriggerID JudgeTransition()
		{
			if (counter <= 0) {	// 既定のループ数に到達.
				return TriggerID.TimeExpire;
			}
			if (CommandReceiveFlags.Instance.CheckCommandReceived(CommandID.Stop))
			{
				return TriggerID.StopCommand;
			}

			return TriggerID.NoTrigger;
		}
	}
}