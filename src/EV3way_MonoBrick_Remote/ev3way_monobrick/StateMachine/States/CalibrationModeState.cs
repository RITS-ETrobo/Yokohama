using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class CalibrationModeState : State
	{
		public CalibrationModeState(EV3body body) : base(body, 4)
		{
		}

		public override void Enter()
		{
			// TODO: show a modeless select dialog
		}

		public override void Do()
		{
			// Nothing to do
		}

		public override void Exit()
		{
			// Nothing to do
		}

		public override TriggerID JudgeTransition()
		{
			// TODO: return trigger for the selection

			return TriggerID.NoTrigger;
		}
	}
}
