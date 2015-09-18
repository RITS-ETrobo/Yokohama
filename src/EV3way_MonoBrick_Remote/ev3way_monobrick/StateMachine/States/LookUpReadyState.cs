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

		sbyte pwmL, pwmR;

		bool isTailAngleStandUp = false; 

		int count = 0;

		int nowAngle;

		sbyte pwmBorder = 0;

		public LookUpReadyState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote("LookUp Ready.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

//			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_STAND_UP);
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_LOOKUPGATE);
		}

		public override void Do()
		{

			_body.motorTail.UpdateTailAngle ();

//			if (!isTailAngleStandUp) {
				
				LogTask.LogRemote ("#### 1 ####");

				sbyte forward = 10;
				sbyte turn = 0;

				int gyroNow = -1 * _body.gyro.GetSensorValue ();
				int thetaL = _body.motorL.GetTachoCount ();
				int theTaR = _body.motorR.GetTachoCount ();

				Balancer.control (
					(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)theTaR, (float)_batteryLevel,
					out pwmL, out pwmR
				);
					
//			}
//
//
//			if (_body.motorTail.IsReachedSubTargetAngle (MotorTail.TAIL_ANGLE_STAND_UP)) {
//
//				LogTask.LogRemote ("#### 2 ####");
//
////				if (count % 2 == 0) {
//									
//					pwmL -= 1;
//					if (pwmL <= pwmBorder) {
//						pwmL = pwmBorder;
//					}
//					pwmR -= 1;
//					if (pwmR <= pwmBorder) {
//						pwmR = pwmBorder;
//					}
////				}
//			
//				nowAngle = MotorTail.TAIL_ANGLE_STAND_UP;
//				isTailAngleStandUp = true;
//
////				count++;
//			}
//
//			if (pwmL == pwmBorder && pwmR == pwmBorder) {
//				LogTask.LogRemote ("#### 3 ####");
//			
//				nowAngle -= 10;
//				if (nowAngle <= MotorTail.TAIL_ANGLE_LOOKUPGATE) {
//					nowAngle = MotorTail.TAIL_ANGLE_LOOKUPGATE;
//				}
//				_body.motorTail.SetMotorAngle (nowAngle);
//			}

			_body.motorL.SetPower (pwmL);
			_body.motorR.SetPower (pwmR);

			// 自己位置の更新
			_body.odm.update(_body.motorL.GetTachoCount(), _body.motorR.GetTachoCount());
		}

		public override void Exit()
		{
			_body.motorL.SetPower (0);
			_body.motorR.SetPower (0);
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.gyro.GetRapidChange ()) {
				return TriggerID.DetectShock;
			}
			if(_body.motorTail.IsReachedSubTargetAngle(MotorTail.TAIL_ANGLE_LOOKUPGATE)){
				return TriggerID.LookUpAngle;
			}
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

