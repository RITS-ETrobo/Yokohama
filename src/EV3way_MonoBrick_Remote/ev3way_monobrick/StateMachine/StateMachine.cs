using System;
using System.Threading;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class StateMachine
	{
		/// <summary>現在の状態</summary>
		private StateID currentState;

		/// <summary>状態の実体</summary>
		private State[] _states;

		/// <summary>状態遷移表</summary>
		private Transition?[,] TransitionMatrix;

		/// <summary>機体</summary>
		private EV3body _body;

		public StateMachine(EV3body body)
		{
			_body = body;

			currentState = StateID.Ready;

			_states = new State[(int)StateID.NumOfState]
			{
				new ReadyState(_body),	// Ready
				new StraightWithLineTraceState(_body),	// Straight1
				new CompleteState(_body)	// Complete
			};

			TransitionMatrix = new Transition?[(int)StateID.NumOfState, (int)TriggerID.NumOfTrigger]
			{
				// TouchSensor

				// 走行準備
				{ new Transition(StateID.Straight1, Nop) },

				// ゴールまで走行
				{ new Transition(StateID.Complete, Nop) },

				// ルックアップゲート用

				// フィギュアL用

				// ...

				// その他
				{ null }
			};
		}

		public void Run()
		{
			// このままループに入ると, 最初のStateのEnter()が実行されない.
			// そのため, 最初のStateだけ特別にここでEnter()を呼び出す.
			_states[(int)currentState].Enter();

			while (true)
			{
				_states[(int)currentState].Do();
				
				TriggerID trigger = _states[(int)currentState].JudgeTransition();

				if (trigger != TriggerID.NoTrigger)
				{
					// トリガー発生

					Transition? transition = TransitionMatrix[(int)currentState, (int)trigger];
					if (transition.HasValue)
					{
						// 状態遷移表に該当する遷移があれば, 遷移させる

						_states[(int)currentState].Exit();

						transition.Value.TransitionMethod ();
						currentState = transition.Value.NextState;

						_states[(int)currentState].Enter();
					}
				}

				Thread.Sleep(_states[(int)currentState].IntervalTime);
			}
		}

		private void Nop()
		{
			// Nothing to do
		}

		/// <summary>遷移メソッドのDelegate</summary>
		public delegate void TransitionMethodDel();

		/// <summary>遷移</summary>
		/// <remarks>次のStateと, 遷移メソッドの組</remarks>
		public struct Transition
		{
			/// <summary>次のStateのStateID</summary>
			public StateID NextState;
			/// <summary>遷移メソッド</summary>
			public TransitionMethodDel TransitionMethod;

			/// <summary>コンストラクタ</summary>
			public Transition(StateID nextState, TransitionMethodDel transitionMethod)
			{
				NextState = nextState;
				TransitionMethod = transitionMethod;
			}
		}
	}
}

