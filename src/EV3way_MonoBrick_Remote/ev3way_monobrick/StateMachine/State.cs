using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public abstract class State
	{
		/// <summary>機体</summary>
		protected EV3body _body;

		public State(EV3body body)
		{
			_body = body;
		}

		/// <summary>任意のStateからこのStateに遷移するときに実行される処理</summary>
		public abstract void Enter();

		/// <summary>このStateにいる間, 繰り返し行われる処理</summary>
		public abstract void Do();

		/// <summary>このStateから任意のStateへ遷移するときの処理</summary>
		public abstract void Exit();

		/// <summary>遷移条件の判定</summary>
		/// <returns>発生したトリガー</returns>
		public abstract TriggerID JudgeTransition();
	}
}

