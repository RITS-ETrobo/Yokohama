using System;
using ETRobocon.EV3;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class LookUpReadyState : State
	{

		//-必要に応じてプライベートフィールドや定数を追加-
		//-メソッドは追加しないでほしい(追加する必要ないはず)-
		private int _batteryLevel;

		public LookUpReadyState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote("LookUp Ready.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_STAND_UP);
		}

		public override void Do()
		{
			sbyte forward = 0;
			sbyte turn = 0;

			_body.motorTail.UpdateTailAngle ();

			int gyroNow = _body.gyro.GetSensorValue();
			int thetaL = _body.motorL.GetTachoCount();
			int theTaR = _body.motorR.GetTachoCount();
//			sbyte pwmL, pwmR;
//			Balancer.control (
//				(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)theTaR, (float)_batteryLevel,
//				out pwmL, out pwmR
//			);
//
//			if (pwmL == 0) {
//				_body.motorL.Brake();
//			} else {
//				_body.motorL.SetPower(pwmL);
//			}
//			if (pwmR == 0) {
//				_body.motorR.Brake();
//			} else {
//				_body.motorR.SetPower(pwmR);
//			}

			_body.motorL.SetPower (0);
			_body.motorR.SetPower (0);

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
//			if(_body.motorTail.IsReachedSubTargetAngle(MotorTail.TAIL_ANGLE_LOOKUPGATE)){
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

