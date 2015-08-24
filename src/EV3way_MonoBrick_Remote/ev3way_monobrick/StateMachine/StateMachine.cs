using System;

namespace ETRobocon.StateMachine
{
	public class StateMachine
	{
		/// <summary>現在の状態</summary>
		private StateID currentState;

		/// <summary>状態の実体</summary>
		private State[] _states;

		public StateMachine ()
		{
			currentState = StateID.Ready;

			_states = new State[(int)StateID.NumOfState]
			{
				new ReadyState(),	// Ready
				new StraightWithLineTraceState(),	// Straight1
				new CompleteState()	// Complete
			};

		}

		public void Run()
		{
			while (true)
			{
				_states[(int)currentState].Do();
				
				TriggerID trigger = _states[(int)currentState].JudgeTransition();

				if (trigger != TriggerID.NoTrigger)
				{
					// トリガー発生
				}

			}
		}
	}
}

