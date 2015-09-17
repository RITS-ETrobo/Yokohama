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

			currentState = StateID.ModeSel;

			//	$\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\StateID.cs
			//	で管理している State が追加/変更/削除された場合は、以下を併せて変更する事
			_states = new State[(int)StateID.NumOfState]
			{
				new ModeSelectState(_body),	// ModeSel
				new ReadyState(_body),	// Ready
				new StraightWithLineTraceState(_body),	// Straight1
				new LookUpReadyState(_body), //LookUpReady
				new LookUpStraightState(_body), //LookUpStaright
				new LookUpTurnState(_body),	//LookUpTurn
				new CompleteState(_body)	// Complete
			};

			TransitionMatrix = new Transition?[(int)StateID.NumOfState, (int)TriggerID.NumOfTrigger]
			{
#if	false	//	この部分は、コメントを含む説明の為に無効化しているので、削除しない事
				{
					//	State : Template(StateIDに置き換える事)
					//	説明 -->
					//	次の場合に、本テーブルのデータを変更する事
					//	1. $\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\TriggerID.cs で管理している Trigger が追加/変更/削除された
					//	2. $\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\StateID.cs で管理している State が追加/変更/削除された
					//	3. テーブル中のTriggerを変更する
					//
					//	テーブルの追加方法
					//	1. Templateをコピーし、StateIDと同じ位置へ挿入する
					//	2. テーブルに関する説明を削除する
					//	3. Stateを記載する
					//
					//	補足
					//	コメント中のタブによるインデントは、StateとTriggerを区別しやすくする為の意図したモノである
					//	<-- 説明
					/*		Trigger	:	TouchSensor	*/	TOUCHSENSOR,
					/*		Trigger	:	RunCommand	*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand	*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock	*/	DETECTSHOCK,
					/*		Trigger	:	Select1 	*/	SELECT1,
					/*		Trigger	:	Select2 	*/	SELECT2,
					/*		Trigger	:	Select3 	*/	SELECT3,
					/*		Trigger	:	Sonar	 	*/	Sonar,
					/*		Trigger : 	LookUpAngle	*/	LookUpAngle,
					/*		Trigger :	Distance	*/	Distance,
					/*		Trigger :	Turn		*/	Turn
				},
#endif	//	false

				{
					//	State	:	ModeSel
					/*		Trigger	:	TouchSensor	*/	T(S.Ready, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	null,
					/*		Trigger	:	Select1 	*/	T(S.Ready, Nop),
					/*		Trigger	:	Select2 	*/	T(S.Complete, Nop),	// キャリブレートステートはまだ無いので, 仮
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
				},
				{
					//	State	:	Ready
					/*		Trigger	:	TouchSensor	*/	T(S.Straight1, Nop),
					/*		Trigger	:	RunCommand	*/	T(S.Straight1, Nop),
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	T(S.Straight1, Nop),
					/*		Trigger	:	Select2 	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
				},

				{
					//	State	:	Straight1
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	T(S.LookUpReady, Nop),
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
				},


				{
					//	State	:	LookUpReady
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	T(S.LookUpStraight1, Nop),
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
				},

				{
					//	State	:	LookUpStraight1
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	T(S.Complete, Nop),
					/*		Trigger :	Turn		*/	null
				},

				{
					//	State	:	LookUpTurn
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
				},
#if	false
				{
					//	State	:	フィギュアL用
					/*		Trigger	:	TouchSensor	*/	TOUCHSENSOR,
					/*		Trigger	:	RunCommand	*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand	*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock	*/	DETECTSHOCK,
					/*		Trigger	:	Select1 	*/	SELECT1,
					/*		Trigger	:	Select2 	*/	SELECT2,
					/*		Trigger	:	Select3 	*/	SELECT3,
					/*		Trigger	:	Sonar	 	*/	Sonar,
					/*		Trigger : 	LookUpAngle	*/	LookUpAngle,
					/*		Trigger :	Distance	*/	Distance,
					/*		Trigger :	Turn		*/	Turn
				},
#endif	//	false

#if	false
				{
					//	State	:	...
					/*		Trigger	:	TouchSensor	*/	TOUCHSENSOR,
					/*		Trigger	:	RunCommand	*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand	*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock	*/	DETECTSHOCK,
					/*		Trigger	:	Select1 	*/	SELECT1,
					/*		Trigger	:	Select2 	*/	SELECT2,
					/*		Trigger	:	Select3 	*/	SELECT3,
					/*		Trigger	:	Sonar	 	*/	Sonar,
					/*		Trigger : 	LookUpAngle	*/	LookUpAngle,
					/*		Trigger :	Distance	*/	Distance,
					/*		Trigger :	Turn		*/	Turn
				},
#endif	//	false

				{
					//	State	:	Complete
					/*		Trigger	:	TouchSensor	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	null,
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null
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

		/// <summary>
		/// ルックアップゲート攻略のために尻尾の角度を設定する
		/// </summary>
		private void setTailAngleForLookUpGate(){
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_LOOKUPGATE);
			_body.motorTail.UpdateTailAngle ();
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

