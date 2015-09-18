using System;
using ETRobocon.EV3;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class LookUpStraightState : State
	{

		//-必要に応じてプライベートフィールドや定数を追加-
		//-メソッドは追加しないでほしい(追加する必要ないはず)-
		private int _batteryLevel;

		//スタート時の総移動距離
		private double distanceOfStart;

		//目標走行距離だけ走ったかどうかのフラグ
		private bool isRunningTargetDistance = false;

		//目標走行距離[mm]
		private const double TARGET_DISTANCE = 300;


		public LookUpStraightState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote ("LookUp Straight");
//			distanceOfStart = _body.odm.TotalMoveDistanceMM;
		}

		public override void Do()
		{
			//直進させる
//			sbyte forward = 50;
//			sbyte turn = 0;

			_body.motorTail.SetBrake ();

//			int gyroNow = _body.gyro.GetSensorValue();
//			int thetaL = _body.motorL.GetTachoCount();
//			int theTaR = _body.motorR.GetTachoCount();
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

			// 自己位置の更新
			_body.odm.update(_body.motorL.GetTachoCount(), _body.motorR.GetTachoCount());

			//30センチ走行したら
//			if((_body.odm.TotalMoveDistanceMM - distanceOfStart) >= TARGET_DISTANCE){
//				isRunningTargetDistance = true;
//			}
		}

		public override void Exit()
		{
			//-Exit()を実装する-
		}

		public override TriggerID JudgeTransition()
		{
//			if (_body.gyro.GetRapidChange ()) {
//				LogTask.LogRemote ("?");
//				return TriggerID.DetectShock;
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
