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

		public StraightWithLineTraceState(EV3body body) : base(body, 2)
		{
		}

		public override void Enter()
		{
			var dialogRun = new InfoDialog("Running", false);
			dialogRun.Show();

			LogTask.LogRemote("EV3 run.");

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();
		}

		public override void Do()
		{
			sbyte forward;
			sbyte turn;

			tail_control(_body, TAIL_ANGLE_DRIVE); // バランス走行用角度に制御

			if (++_counter >= 40/4) {
				_alert = sonar_alert (_body);
				_counter = 0;
			}
			if (_alert) {
				forward = 0;
				turn = 0;
			} else {
				forward = 50;
				turn = (_body.color.Read () >= (LIGHT_BLACK + LIGHT_WHITE) / 2) ? (sbyte)50 : (sbyte)-50;
			}

			int gyroNow = _body.gyro.Read();
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
		}

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.touch.IsPressed())
			{
				return TriggerID.TouchSensor;
			}

			return TriggerID.NoTrigger;
		}
	}
}

