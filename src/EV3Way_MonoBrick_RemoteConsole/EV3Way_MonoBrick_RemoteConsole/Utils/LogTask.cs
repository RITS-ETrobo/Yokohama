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

		/// <summary>ログとして受信できないデータが送られてきたときの代替文字列</summary>
		private const string INVALID_RECEIVE_DATA = "(Invalid Data)";

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
					if (data is string)	// 文字列
					{
						Console.WriteLine((string)data);
					}
					else if (data is Array)	// それ以外の組み込み型
					{
						Array array = (Array)data;
						string str = "";
						foreach (object obj in array)
						{
							if (obj is bool) { str += ((bool)obj).ToString(); }
							else if (obj is sbyte) { str += ((sbyte)obj).ToString(); }
							else if (obj is short) { str += ((short)obj).ToString(); }
							else if (obj is int) { str += ((int)obj).ToString(); }
							else if (obj is long) { str += ((long)obj).ToString(); }
							else if (obj is byte) { str += ((byte)obj).ToString(); }
							else if (obj is ushort) { str += ((ushort)obj).ToString(); }
							else if (obj is uint) { str += ((uint)obj).ToString(); }
							else if (obj is ulong) { str += ((ulong)obj).ToString(); }
							else if (obj is decimal) { str += ((decimal)obj).ToString(); }
							else if (obj is char) { str += ((char)obj).ToString(); }
							else if (obj is float) { str += ((float)obj).ToString(); }
							else if (obj is double) { str += ((double)obj).ToString(); }
							else { str += INVALID_RECEIVE_DATA; }

							str += " ";
						}
						Console.WriteLine(str);
					}
					else
					{
						Console.WriteLine(INVALID_RECEIVE_DATA);
					}

					continue;
				}

				Thread.Sleep(LOOP_INTERVAL);
			}
		}
	}
}
