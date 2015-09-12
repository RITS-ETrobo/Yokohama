using System;
using System.Threading;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	// 状態遷移表で, 文字数短縮のためのエイリアス
	using S = StateID;

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
				// TouchSensor,        RunCommand,          StopCommand         DetectShock          Select1             Select2              Select3

				// 走行準備
				{ T(S.Straight1, Nop), T(S.Straight1, Nop), null,               T(S.Complete, Nop),	 T(S.Straight1, Nop), T(S.Complete, Nop), null },

				// ゴールまで走行
				{ T(S.Complete, Nop),  null,                T(S.Complete, Nop), T(S.Complete, Nop),	 null,               null,                null },

				// ルックアップゲート用

				// フィギュアL用

				// ...

				// その他
				{ null,                null,                null,               null,                null,               null,                null  }
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

		/// <summary>状態遷移表で, 文字数短縮のためのメソッド</summary>
		/// <param name="nextState">次のステートのID</param>
		/// <param name="transitionMethod">遷移メソッド</param>
		private Transition T(StateID nextState, TransitionMethodDel transitionMethod)
		{
			return new Transition(nextState, transitionMethod);
		}
	}
}

