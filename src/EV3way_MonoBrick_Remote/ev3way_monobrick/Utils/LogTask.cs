using System;
using System.Threading;

namespace ETRobocon.Utils
{
	/// <summary>ログ機能(送信)</summary>
	public class LogTask
	{
		/// <summary>ログ機能のインスタンス</summary>
		private static LogTask _instance = new LogTask();

		/// <summary>ログタスクが実行されているスレッド</summary>
		private Thread _logThread;

		private LogTask ()
		{
		}


		/// <summary>ログ送信タスクを開始する</summary>
		/// <remarks>通信確立も行うので, 接続待ち状態となる.</remarks>
		public static void Run()
		{
			LogTask._instance._logThread = new Thread (LogTask._instance.Loop);
			LogTask._instance._logThread.Priority = ThreadPriority.Lowest;
			LogTask._instance._logThread.Start ();
		}

		/// <summary>ログをリモートで送信する</summary>
		/// <remarks>ログは一旦キューに積まれ, 非同期で送信される.</remarks>
		/// <param name="data">送るデータ</param>
		public static void LogRemote(object data)
		{
		}

		/// <summary>タスクのメインループ</summary>
		private void Loop()
		{
			while (true) {
				
			}
		}
	}
}

