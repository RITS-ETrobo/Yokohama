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

//		int targetAngle = MotorTail.TAIL_ANGLE_LOOKUPGATE;
		int targetAngle = 90;


		sbyte pwmBorder = 0;

		enum state{
			Balance = 0,
			Foward,
			Sit,
			Done
		}
		state currentState = state.Balance;




		public LookUpReadyState(EV3body body /*, -必要に応じて引数を追加-*/) : base(body, 2)
		{
			//-コンストラクタを実装する-
		}

		public override void Enter()
		{
			LogTask.LogRemote("LookUp Ready.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

//			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_STAND_UP - 5, 10);
			_body.motorTail.SetMotorAngle (targetAngle,10);
		}

		public override void Do()
		{
			sbyte forward;
			sbyte turn;
			int gyroNow = _body.gyro.GetSensorValue();
			int thetaL = _body.motorL.GetTachoCount ();
			int thetaR = _body.motorR.GetTachoCount ();
		
			if (currentState == state.Balance) {

				//その場に止まってバランス制御を行う
				forward = 0;
				turn = 0;

				//しっぽの角度を90度まで10段階を経て設定する
				_body.motorTail.UpdateTailAngle ();

				Balancer.control (
					(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)thetaR, (float)_batteryLevel,
					out pwmL, out pwmR
				);

				//4msecループ×500回=2秒間ほどバランス制御
				if (count++ >= 500) {
					currentState = state.Foward;
					LogTask.LogRemote ("### forward ###");
					count = 0;
				}

			} else if (currentState == state.Foward) {

				//ジャイロセンサの値をひっくり返すことで後ろにこかす
				forward = 20;
//				forward = 10;
				turn = 0;

				gyroNow = -1 * _body.gyro.GetSensorValue();

				//UpdateTailAngle()だとしっかりと固定されず
				//こけた衝撃でしっぽが曲がり支えきれないことがあるため
				//Brake()メソッドを呼び出す
				_body.motorTail.SetBrake ();

				Balancer.control (
					(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)thetaR, (float)_batteryLevel,
					out pwmL, out pwmR
				);
			
				currentState = state.Sit;
				LogTask.LogRemote ("## Sit ##");

//			} else if (currentState == state.Sit) {
//
//				//TODO うまく角度を減らせてない
//
//				//バランス制御をなくしてタイヤの回転も止める
//				pwmL = 0;
//				pwmR = 0;
//
//				//4msec×500回=2秒間ごとに角度を減らす
//				if (count++ >= 500) {
//
//					//徐々に目標の角度に調整する
//					LogTask.LogRemote(_body.motorTail.GetCurrentAngle());
////					targetAngle = _body.motorTail.GetCurrentAngle();
////					if (targetAngle < MotorTail.TAIL_ANGLE_LOOKUPGATE) {
////						targetAngle = MotorTail.TAIL_ANGLE_LOOKUPGATE;//最低値はルックアップゲート用角度
////						LogTask.LogRemote("## Target angle ##");
////					}
//
////					_body.motorTail.moveTail (targetAngle);
//					_body.motorTail.moveTail (MotorTail.TAIL_ANGLE_LOOKUPGATE);
//					_body.motorTail.SetBrake ();
//
//					if (_body.motorTail.IsReachedSubTargetAngle (MotorTail.TAIL_ANGLE_LOOKUPGATE)) {
////						currentState = state.Done;
//						LogTask.LogRemote("## Done ##");
//					}
//					count = 0;
//				}
			}

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
			//ジャイロがあると後ろにこかした時に引っかかる恐れがあるのでコメントアウト
//			if (_body.gyro.GetRapidChange ()) {
//				return TriggerID.DetectShock;
//			}
//			if (currentState == state.Done) {
//				return TriggerID.LookUpAngle;				
//			}
			if (currentState == state.Sit) {
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

