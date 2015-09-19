using System;
using ETRobocon.EV3;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class LookUpReadySetTailLUGState : State
	{
		const int COUNT_SETLUG_OK = 300;
		int counter;

		public LookUpReadySetTailLUGState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote("LookUp Ready Set Tail LUG.");

			counter = COUNT_SETLUG_OK;

			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_LOOKUPGATE,(int)(COUNT_SETLUG_OK*0.5));
		}

		public override void Do()
		{
			_body.motorTail.UpdateTailAngle ();

			counter--;
		}

		public override void Exit()
		{
			_body.motorL.SetPower (0);
			_body.motorR.SetPower (0);
		}

		public override TriggerID JudgeTransition()
		{
			if (counter <= 0) {
				return TriggerID.TimeExpire;
			}
			if (_body.touch.DetectReleased() )	//DetectShockもここに入る. (未対応)
			{
				return TriggerID.TouchSensor;
			}
			if (CommandReceiveFlags.Instance.CheckCommandReceived(CommandID.Stop))
			{
				return TriggerID.StopCommand;
			}

			return TriggerID.NoTrigger;
		}
	}
}

