using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class ReadyState : State
	{
		public ReadyState(EV3body body) : base(body)
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

