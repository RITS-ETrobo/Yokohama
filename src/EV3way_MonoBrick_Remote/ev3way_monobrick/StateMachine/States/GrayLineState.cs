using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class GrayLineState : State
	{
		public GrayLineState(EV3body body) : base(body, 2)
		{
		}

		public override void Enter()
		{
		}

		public override void Do()
		{
		}

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			return TriggerID.NoTrigger;
		}
	}
}
