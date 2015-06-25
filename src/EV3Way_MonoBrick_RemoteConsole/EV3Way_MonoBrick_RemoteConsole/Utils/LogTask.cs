using System;
using System.Threading;

namespace EV3Way_MonoBrick_RemoteConsole.Utils
{
	/// <summary>ログ機能(受信と表示)</summary>
	public class LogTask
	{
		/// <summary>ログ機能のインスタンス</summary>
		private static LogTask _instance = new LogTask();

		/// <summary>ログタスクが実行されているスレッド</summary>
		private Thread _logThread;

		/// <summary>ログタスクのメインループのSleep時間</summary>
		private const int LOOP_INTERVAL = 16;

		private LogTask ()
		{
		}


		/// <summary>ログ受信・表示タスクを開始する</summary>
		public static void Run()
		{
			ProtocolProcessorForPC.Connect();

			LogTask._instance._logThread = new Thread (LogTask._instance.Loop);
			LogTask._instance._logThread.Priority = ThreadPriority.Lowest;
			LogTask._instance._logThread.Start ();
		}

		/// <summary>タスクのメインループ</summary>
		private void Loop()
		{
			object data;

			while (true)
			{
				if (ProtocolProcessorForPC.Instance.ReceiveData(out data))
				{
					// 受信したログを表示
				}

				Thread.Sleep(LOOP_INTERVAL);
			}
		}
	}
}
