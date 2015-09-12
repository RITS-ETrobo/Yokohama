using System;
using System.Diagnostics;	// for Debugger
using MonoBrickFirmware.Display;	// for Lcd
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class CompleteState : State
	{
		public CompleteState(EV3body body) : base(body, 4)
		{
		}

		public override void Enter()
		{
			LogTask.LogRemote("EV3 stopped.");

			_body.motorL.Off ();
			_body.motorR.Off ();
			_body.motorTail.TurnOff ();

			// 自己位置推定ログのファイルへの書き出し
			_body.odm.outputLogToFile ();

			Lcd.Instance.Clear ();
			Lcd.Instance.Update ();

			if (Debugger.IsAttached) {
				Brick.ExitToMenu (); // MonoBrickメインメニューへ戻る
			}
//
//			_body.gyro.PrintLogs ();
		}

		public override void Do()
		{
		}

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			return TriggerID.NoTrigger;
		}
	}
}

