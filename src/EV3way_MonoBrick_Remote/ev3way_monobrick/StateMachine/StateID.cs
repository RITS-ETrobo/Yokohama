using System;

namespace ETRobocon.StateMachine
{
	public enum StateID
	{
		// 走行準備
		Ready,
		Start,

		// 走行
		Straight1,	///< 最初の直線区間

		// ルックアップゲート用

		// フィギュアL用

		// その他
		Complete,	///< 完走 (走行停止)

		NumOfState
	}
}

