using System;

namespace ETRobocon.StateMachine
{
	public class ReadyState : State
	{
		public ReadyState()
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

