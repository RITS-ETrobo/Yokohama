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
#if	false	//	この部分は、コメントを含む説明の為に無効化しているので、削除しない事
				{
					//	State : Template
					//	説明 -->
					//	次の場合に、本テーブルのデータを変更する事
					//	1. $\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\TriggerID.cs で管理している Trigger が追加/変更/削除された
					//	2. State を追加/変更/削除する
					//	テーブルの追加方法
					//	1. Templateをコピーし、追加したい箇所へ貼り付ける
					//	2. テーブルに関する説明を削除する
					//	3. Stateを記載する
					//	<-- 説明
					/*	TouchSensor	*/	TOUCHSENSOR,
					/*	RunCommand	*/	RUNCOMMAND,
					/*	StopCommand	*/	STOPCOMMAND,
					/*	DetectShock	*/	DETECTSHOCK,
					/*	Select1 	*/	SELECT1,
					/*	Select2 	*/	SELECT2,
					/*	Select3 	*/	SELECT3
				},
#endif	//	false

				{
					//	State : 走行準備
					/*	TouchSensor	*/	T(S.Straight1, Nop),
					/*	RunCommand	*/	T(S.Straight1, Nop),
					/*	StopCommand	*/	null,
					/*	DetectShock	*/	T(S.Complete, Nop),
					/*	Select1 	*/	T(S.Straight1, Nop),
					/*	Select2 	*/	T(S.Complete, Nop),
					/*	Select3 	*/	null
				},

				{
					//	State : ゴールまで走行
					/*	TouchSensor	*/	T(S.Complete, Nop),
					/*	RunCommand	*/	null,
					/*	StopCommand	*/	T(S.Complete, Nop),
					/*	DetectShock	*/	T(S.Complete, Nop),
					/*	Select1 	*/	null,
					/*	Select2 	*/	null,
					/*	Select3 	*/	null
				},

#if	false
				{
					//	State : ルックアップゲート
					/*	TouchSensor	*/	TOUCHSENSOR,
					/*	RunCommand	*/	RUNCOMMAND,
					/*	StopCommand	*/	STOPCOMMAND,
					/*	DetectShock	*/	DETECTSHOCK,
					/*	Select1 	*/	SELECT1,
					/*	Select2 	*/	SELECT2,
					/*	Select3 	*/	SELECT3
				},
#endif	//	false

#if	false
				{
					//	State : フィギュアL用
					/*	TouchSensor	*/	TOUCHSENSOR,
					/*	RunCommand	*/	RUNCOMMAND,
					/*	StopCommand	*/	STOPCOMMAND,
					/*	DetectShock	*/	DETECTSHOCK,
					/*	Select1 	*/	SELECT1,
					/*	Select2 	*/	SELECT2,
					/*	Select3 	*/	SELECT3
				},
#endif	//	false

#if	false
				{
					//	State : ...
					/*	TouchSensor	*/	TOUCHSENSOR,
					/*	RunCommand	*/	RUNCOMMAND,
					/*	StopCommand	*/	STOPCOMMAND,
					/*	DetectShock	*/	DETECTSHOCK,
					/*	Select1 	*/	SELECT1,
					/*	Select2 	*/	SELECT2,
					/*	Select3 	*/	SELECT3
				},
#endif	//	false

				{
					//	State : その他
					/*	TouchSensor	*/	null,
					/*	RunCommand	*/	null,
					/*	StopCommand	*/	null,
					/*	DetectShock	*/	null,
					/*	Select1 	*/	null,
					/*	Select2 	*/	null,
					/*	Select3 	*/	null
				}
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

