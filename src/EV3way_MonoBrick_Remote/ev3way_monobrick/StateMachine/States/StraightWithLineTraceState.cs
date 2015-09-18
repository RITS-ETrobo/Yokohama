using System;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class StraightWithLineTraceState : State
	{
		// MainClassのrun()より移してきた
		private int _batteryLevel;
		private int _counter = 0;
		private bool _alert = false;

		private LineDetector _ld;

		/// <summary>
		/// ループ時間調整用定数
		/// 1ループの中で行うダミーループ数を示す.
		/// </summary>
		private const int LOOP_DELAY = 29;

		public StraightWithLineTraceState(EV3body body) : base(body, 1)
		{
			_ld = new LineDetectorOld (0, 60, LineDetector.LineEdge.Left, 20.0f, 0, 80.0f ); // TODO: kp, ki, kdの値を変えて調査してください。
		}

		public override void Enter()
		{
			var dialogRun = new InfoDialog("Running", false);
			dialogRun.Show();

			LogTask.LogRemote("EV3 run.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			// 走行開始前にタイヤが動いていると自己位置推定に誤差が出てくるのでTachoCountの値をリセットする]
			//  - ココじゃなくてStateMachine.cs内の走行準備からの遷移であるnew Transition(StateID.Straight1, Nop)のNopのところでやるべき？
			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_DRIVE);	//バランス走行用角度に制御
		}

		public override void Do()
		{
			sbyte forward;
			sbyte turn;

			_body.motorTail.UpdateTailAngle ();

			if (++_counter >= 40/4) {
				_alert = sonar_alert (_body);
				_counter = 0;
			}
			if (_alert) {
				forward = 0;
				turn = 0;
			} else {
				forward = 50;
				turn = _ld.CalculateTurn(_body.color.Read());
			}

			int gyroNow = _body.gyro.GetSensorValue();
			int thetaL = _body.motorL.GetTachoCount();
			int theTaR = _body.motorR.GetTachoCount();
			sbyte pwmL, pwmR;
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

			// 自己位置の更新
			_body.odm.update(_body.motorL.GetTachoCount(), _body.motorR.GetTachoCount());

			// ループ時間の調整
			for (int i = 0; i < LOOP_DELAY; i++) {
				double dummy;
				dummy = i / 100.1F;
				dummy = dummy * 0.0F;
				i = i + (int)dummy;
			}
		}

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.gyro.GetRapidChange ()) {
				return TriggerID.DetectShock;
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

