using System;
using System.Threading;
using ETRobocon.EV3;
using ETRobocon.Utils;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.StateMachine
{
	public class StartState : State
	{
		// MainClassのrun()より移してきた
		private int _batteryLevel;

		private const int START_COUNT = 5;	// 安定処理するまでのループ数
		private int counter;	// ループ回数カウンター

		public StartState(EV3body body) : base(body, 2)
		{
			counter = START_COUNT;
		}

		public override void Enter()
		{
			
			// 走行開始前にタイヤが動いていると自己位置推定に誤差が出てくるのでTachoCountの値をリセットする]
			//  - ココじゃなくてStateMachine.cs内の走行準備からの遷移であるnew Transition(StateID.Straight1, Nop)のNopのところでやるべき？
			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			// スタート時の尻尾角度に制御
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_START);
		}

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

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			if (counter <= 0) {	// 既定のループ数に到達.
				return TriggerID.TimeExpire;
			}
			else if (_body.touch.IsPressed())
			{
				return TriggerID.TouchSensor;
			}
			else if (CommandReceiveFlags.Instance.CheckCommandReceived(CommandID.Stop))
			{
				return TriggerID.StopCommand;
			}

			return TriggerID.NoTrigger;
		}
	}
}