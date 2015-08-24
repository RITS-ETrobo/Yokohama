using System;

namespace ETRobocon.StateMachine
{
	public class CompleteState : State
	{
		public CompleteState()
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

