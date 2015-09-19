using System;
using System.Threading;
using ETRobocon.EV3;
using ETRobocon.Utils;
using MonoBrickFirmware.Display.Dialogs;

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
				new CalibrationModeState(_body),	// Calib
				new ReadyState(_body),	// Ready
				new StraightWithLineTraceState(_body, 7500),	// Straight1
				new GrayLineState(_body, 500),	// GrayL1
				new StraightWithLineTraceState(_body, 10000),	// StraightL2
				new LookUpReadyState(_body), //LookUpReady
				new LookUpReadySetTailLUGState(_body), //LookUpReadySetLUG
				new LookUpStraightState(_body), //LookUpStaright1
				new LookUpTurnState(_body),	//LookUpTurn1
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
					/*		Trigger	:	TouchSensor		*/	TOUCHSENSOR,
					/*		Trigger	:	ReachDistance	*/	REACHDISTANCE,
					/*		Trigger	:	RunCommand		*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand		*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock		*/	DETECTSHOCK,
					/*		Trigger	:	Select1 		*/	SELECT1,
					/*		Trigger	:	Select2 		*/	SELECT2,
					/*		Trigger	:	Select3			*/	SELECT3,
					/*		Trigger	:	Select4 		*/	SELECT4,
					/*		Trigger	:	Sonar	 		*/	SONAR,
					/*		Trigger : 	LookUpAngle		*/	LOOKUPANGLE,
					/*		Trigger :	Distance		*/	DISTANCE,
					/*		Trigger :	Turn			*/	TURN,
					/*		Trigger :	TimeExpire		*/	TimeExpire
				},
#endif	//	false

				#region 走行準備
				{
					//	State	:	ModeSel
					/*		Trigger	:	TouchSensor	*/	T(S.Ready, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	null,
					/*		Trigger	:	Select1 	*/	T(S.Ready, Nop),
					/*		Trigger	:	Select2 	*/	T(S.Calib, Nop),
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				{
					//	State	:	Calib
					/*		Trigger	:	TouchSensor	*/	null,
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	null,
					/*		Trigger	:	Select1 	*/	T(S.Calib, CalibWhite),
					/*		Trigger	:	Select2 	*/	T(S.Calib, CalibBlack),
					/*		Trigger	:	Select3 	*/	T(S.Calib, CalibGray),
					/*		Trigger	:	Select4 	*/	T(S.ModeSel, Nop),
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				{
					//	State	:	Ready
					/*		Trigger	:	TouchSensor	*/	T(S.Straight1, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	T(S.Straight1, Nop),
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	T(S.Straight1, Nop),
					/*		Trigger	:	Select2 	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				#endregion

				{
					//	State	:	Straight1
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	T(S.GrayL1, Nop),
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	T(S.LookUpReady, Nop),
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				{
					//	State	:	GrayL1
					/*		Trigger	:	TouchSensor		*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	T(S.StraightL2, Nop),
					/*		Trigger	:	RunCommand		*/	null,
					/*		Trigger	:	StopCommand		*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock		*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 		*/	null,
					/*		Trigger	:	Select2 		*/	null,
					/*		Trigger	:	Select3 		*/	null,
					/*		Trigger	:	Select4 		*/	null,
					/*		Trigger	:	Sonar			*/	T(S.LookUpReady, Nop),
					/*		Trigger : 	LookUpAngle		*/	null,
					/*		Trigger :	Distance		*/	null,
					/*		Trigger :	Turn			*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				{
					//	State	:	StraightL2
					/*		Trigger	:	TouchSensor		*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand		*/	null,
					/*		Trigger	:	StopCommand		*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock		*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 		*/	null,
					/*		Trigger	:	Select2 		*/	null,
					/*		Trigger	:	Select3 		*/	null,
					/*		Trigger	:	Select4 		*/	null,
					/*		Trigger	:	Sonar			*/	T(S.LookUpReady, Nop),
					/*		Trigger : 	LookUpAngle		*/	null,
					/*		Trigger :	Distance		*/	null,
					/*		Trigger :	Turn			*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},

				#region ルックアップゲート
				{
					//	State	:	LookUpReady
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	T(S.LookUpStraight1, Nop),
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},

				{
					//	State	:	LookUpReadySetTailLUG
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	T(S.LookUpStraight1, Nop)
				},

				{
					//	State	:	LookUpStraight1
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	T(S.Complete, Nop),
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},

				{
					//	State	:	LookUpTurn1
					/*		Trigger	:	TouchSensor	*/	T(S.Complete, Nop),
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	T(S.Complete, Nop),
					/*		Trigger	:	DetectShock	*/	T(S.Complete, Nop),
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
				},
				#endregion

#if	false
				{
					//	State	:	フィギュアL用
					/*		Trigger	:	TouchSensor		*/	TOUCHSENSOR,
					/*		Trigger	:	ReachDistance	*/	REACHDISTANCE,
					/*		Trigger	:	RunCommand		*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand		*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock		*/	DETECTSHOCK,
					/*		Trigger	:	Select1			*/	SELECT1,
					/*		Trigger	:	Select2 		*/	SELECT2,
					/*		Trigger	:	Select3 		*/	SELECT3,
					/*		Trigger	:	Select4 		*/	SELECT4,
					/*		Trigger	:	Sonar		 	*/	SONAR,
					/*		Trigger : 	LookUpAngle		*/	LOOKUPANGLE,
					/*		Trigger :	Distance		*/	DISTANCE,
					/*		Trigger :	Turn			*/	TURN,
					/*		Trigger :	TimeExpire		*/	TimeExpire
				},
#endif	//	false

#if	false
				{
					//	State	:	...
					/*		Trigger	:	TouchSensor	*/	TOUCHSENSOR,
					/*		Trigger	:	ReachDistance	*/	REACHDISTANCE,
					/*		Trigger	:	RunCommand	*/	RUNCOMMAND,
					/*		Trigger	:	StopCommand	*/	STOPCOMMAND,
					/*		Trigger	:	DetectShock	*/	DETECTSHOCK,
					/*		Trigger	:	Select1 	*/	SELECT1,
					/*		Trigger	:	Select2 	*/	SELECT2,
					/*		Trigger	:	Select3 	*/	SELECT3,
					/*		Trigger	:	Select4 	*/	SELECT4,
					/*		Trigger	:	Sonar	 	*/	Sonar,
					/*		Trigger : 	LookUpAngle	*/	LookUpAngle,
					/*		Trigger :	Distance	*/	Distance,
					/*		Trigger :	Turn		*/	Turn,
					/*		Trigger :	TimeExpire	*/	TimeExpire
				},
#endif	//	false

				{
					//	State	:	Complete
					/*		Trigger	:	TouchSensor	*/	null,
					/*		Trigger	:	ReachDistance	*/	null,
					/*		Trigger	:	RunCommand	*/	null,
					/*		Trigger	:	StopCommand	*/	null,
					/*		Trigger	:	DetectShock	*/	null,
					/*		Trigger	:	Select1 	*/	null,
					/*		Trigger	:	Select2 	*/	null,
					/*		Trigger	:	Select3 	*/	null,
					/*		Trigger	:	Select4 	*/	null,
					/*		Trigger	:	Sonar	 	*/	null,
					/*		Trigger : 	LookUpAngle	*/	null,
					/*		Trigger :	Distance	*/	null,
					/*		Trigger :	Turn		*/	null,
					/*		Trigger :	TimeExpire	*/	null
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

		private void CalibWhite()
		{
			_body.color.CalibrateWhite();
			Utils.LogTask.LogRemote("Calib White :");
			Utils.LogTask.LogRemote(_body.color.WhiteSensorValue);
		}

		private void CalibBlack()
		{
			_body.color.CalibrateBlack();
			Utils.LogTask.LogRemote("Calib Black :");
			Utils.LogTask.LogRemote(_body.color.BlackSensorValue);
		}

		private void CalibGray()
		{
			_body.color.CalibrateGray();
			Utils.LogTask.LogRemote("Calib Gray :");
			Utils.LogTask.LogRemote(_body.color.GraySensorValue);
		}

		/// <summary>準備状態から走行状態へ遷移するときの処理</summary>
		private void TransReadyToRun()
		{
			var dialogRun = new InfoDialog("Running", false);
			dialogRun.Show();

			LogTask.LogRemote("EV3 run.");

			// 走行開始前にタイヤが動いていると自己位置推定に誤差が出てくるのでTachoCountの値をリセットする
			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();
			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_DRIVE);	//バランス走行用角度に制御
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

