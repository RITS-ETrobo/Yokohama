using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class CompleteState : State
	{
		public CompleteState(EV3body body) : base(body, 4)
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

