using System;
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class ModeSelectState : State
	{
		public ModeSelectState(EV3body body) : base(body, 4)
		{
		}

		public override void Enter()
		{
			// TODO: show a modeless selection dialog
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
			// TODO: return trigger for touch sensor
			// TODO: return trigger for the selection

			return TriggerID.NoTrigger;
		}
	}
}
