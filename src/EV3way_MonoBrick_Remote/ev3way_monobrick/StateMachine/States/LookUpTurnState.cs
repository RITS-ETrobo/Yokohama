using System;
using ETRobocon.EV3;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class LookUpTurnState : State
	{

		//-必要に応じてプライベートフィールドや定数を追加-
		//-メソッドは追加しないでほしい(追加する必要ないはず)-
		private int _batteryLevel;

		private const double TARGET_TURN = 180.0; //目標値(要調整)

		public LookUpTurnState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote("LookUp Turn.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();
		}

		public override void Do()
		{
			_body.motorTail.UpdateTailAngle ();

			_body.motorL.SetPower (10);
			_body.motorR.SetPower (-10);

			// 自己位置の更新
			_body.odm.update(_body.motorL.GetTachoCount(), _body.motorR.GetTachoCount());
		}

		public override void Exit()
		{

		}

		public override TriggerID JudgeTransition()
		{
			if (_body.gyro.GetRapidChange ()) {
				return TriggerID.DetectShock;
			}
//			if(_body.motorTail.(MotorTail.TAIL_ANGLE_LOOKUPGATE)){
//				return TriggerID.LookUpAngle;
//			}
			if (_body.touch.DetectReleased())
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

