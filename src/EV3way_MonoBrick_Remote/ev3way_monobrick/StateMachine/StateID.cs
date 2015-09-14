using System;

namespace ETRobocon.StateMachine
{
	///	<summary>走行に関するState</summary>
	///	<remarks>
	///	Stateを変更した時は、同時に
	///	$\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\StateMachine.cs
	///	の以下のデータを変更する必要がある
	///	_states = new State[(int)StateID.NumOfState]
	///	TransitionMatrix = new Transition?[(int)StateID.NumOfState, (int)TriggerID.NumOfTrigger]
	///	</remarks>
	public enum StateID
	{
		// 走行準備
		Calib,	///< キャリブレーションモード
		Ready,

		// 走行
		Straight1,	///< 最初の直線区間

		// ルックアップゲート用

		// フィギュアL用

		// その他
		Complete,	///< 完走 (走行停止)

		NumOfState
	}
}

