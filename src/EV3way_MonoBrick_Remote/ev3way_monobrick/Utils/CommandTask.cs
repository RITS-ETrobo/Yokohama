using System;
using System.Threading;	// for Thread

namespace ETRobocon.Utils
{
	/// <summary>コマンド受信・実行タスク</summary>
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
			ProtocolProcessorForEV3.Connect();

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

