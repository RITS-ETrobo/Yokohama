using System;
using System.Threading;	// for Thread

using ETRobocon.Utils;	// for ProtocolProcessorForPC

namespace EV3Way_MonoBrick_RemoteConsole.Utils
{
	/// <summary>コマンド入力・送信タスク</summary>
	public class CommandTask
	{
		/// <summary>コマンドタスクのインスタンス</summary>
		private static CommandTask _Instance = new CommandTask();

		/// <summary>コマンドタスクが実行されているスレッド</summary>
		private Thread _CommandThread;

		/// <summary>コマンドタスクのメインループのSleep時間</summary>
		private const int LOOP_INTERVAL = 16;

		private CommandTask()
		{
		}

		/// <summary>コマンドタスクの開始</summary>
		public static void Run()
		{
			ProtocolProcessorForPC.Connect();

			CommandTask._Instance._CommandThread = new Thread(CommandTask._Instance.Loop);
			CommandTask._Instance._CommandThread.Priority = ThreadPriority.Lowest;
			CommandTask._Instance._CommandThread.Start();
		}

		/// <summary>コマンドタスクのメインループ</summary>
		private void Loop()
		{
			while (true)
			{
				Thread.Sleep(LOOP_INTERVAL);
			}
		}
	}
}

