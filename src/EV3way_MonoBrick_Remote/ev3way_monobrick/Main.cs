using System;
using ETRobocon.EV3;
using ETRobocon.StateMachine;

///	2輪倒立振子ライントレースロボットの MonoBrick 用 c# プログラム
namespace ETRobocon.EV3
{
	class MainClass
	{
		public static void Main()
		{
			// 構造体の宣言と初期化
			var body = new EV3body ();

			EV3body.init (ref body);

			var state_machine = new StateMachine.StateMachine(body);
			state_machine.Run();
		}
	}
}

