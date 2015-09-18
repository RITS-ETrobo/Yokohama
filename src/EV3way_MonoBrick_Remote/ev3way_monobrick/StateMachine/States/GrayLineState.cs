using System;
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class GrayLineState : State
	{
		// MainClassのrun()より移してきた
		private int _batteryLevel;
		private int _counter = 0;
		private bool _alert = false;

		private const double DISTANCE_DIFFERENCE = 500;
		private double _startDistance;
		private double _targetDistance;

		private LineDetector _ld;

		public GrayLineState(EV3body body) : base(body, 2)
		{
			_ld = new LineDetectorOld (18, 45, LineDetector.LineEdge.Left, 25.0f, 0, 55.0f );
		}

		public override void Enter()
		{
			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			_startDistance = _body.odm.TotalMoveDistanceMM;
			_targetDistance = _startDistance + DISTANCE_DIFFERENCE;
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
				forward = 25;
				turn = _ld.CalculateTurn(_body.color.ReadSensorValue());
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
			if (_body.odm.TotalMoveDistanceMM >= _targetDistance)
			{
				return TriggerID.ReachDistance;
			}

			return TriggerID.NoTrigger;
		}
	}
}
