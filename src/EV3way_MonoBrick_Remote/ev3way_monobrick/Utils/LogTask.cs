using System;
using System.Collections;	// for Queue
using System.Threading;

namespace ETRobocon.Utils
{
	/// <summary>ログ機能が使用不可となるエラー</summary>
	/// <remarks>NoError以外が設定されると, それ以降ログ機能が使用できなくなる.(復帰不可)</remarks>
	public enum ErrorType
	{
		NoError,		///< 正常動作中
		NetworkError,	///< EV3-PC間の接続エラー
		MemoryOver,		///< キューに積みすぎてメモリオーバー
		OtherError		///< その他のエラー
	};

	/// <summary>ログ機能(送信)</summary>
	public class LogTask
	{
		/// <summary>ログ機能のインスタンス</summary>
		private static LogTask _instance = new LogTask();

		/// <summary>ログタスクが実行されているスレッド</summary>
		private Thread _logThread;

		/// <summary>ログタスクのメインループのSleep時間</summary>
		private const int LOOP_INTERVAL = 16;

		/// <summary>ログを溜めておくQueue</summary>
		private Queue _logBuffer;

		/// <summary>ログのEnqueue, Dequeueを排他させるためのLock</summary>
		private object _logBufferLock = new object();

		/// <summary><see cref=" Enable"/>のフィールド</summary>
		private bool _enable = false;

		/// <summary>ログ送信の有効/無効</summary>
		/// <remarks>無効にしても, ログのキューイングはできる</remarks>
		public static bool Enable
		{
			get
			{
				return LogTask._instance._enable;
			}
			set
			{
				LogTask._instance._enable = value;
			}
		}

		/// <summary>エラーの発生状況</summary>
		public ErrorType ErrorStatus { get; private set; }

		private LogTask ()
		{
			_logBuffer = new Queue();
			_enable = false;
			ErrorStatus = ErrorType.NoError;
		}


		/// <summary>ログ送信タスクを開始する</summary>
		/// <remarks>通信確立も行うので, 接続待ち状態となる.</remarks>
		public static void Run()
		{
			try
			{
				if (LogTask._instance.ErrorStatus == ErrorType.NoError)
				{
					ProtocolProcessorForEV3.Connect();

					LogTask._instance._logThread = new Thread(LogTask._instance.Loop);
					LogTask._instance._logThread.Priority = ThreadPriority.Lowest;
					LogTask._instance._logThread.Start();
				}
			}
			catch (Exception)
			{
				LogTask._instance.ErrorStatus = ErrorType.OtherError;
				// TODO: ログファイルへの出力
			}
		}

		/// <summary>ログをリモートで送信する</summary>
		/// <remarks>ログは一旦キューに積まれ, 非同期で送信される.</remarks>
		/// <param name="data">送るデータ</param>
		public static void LogRemote(object data)
		{
			try
			{
				if (LogTask._instance.ErrorStatus == ErrorType.NoError)
				{
					lock (LogTask._instance._logBufferLock)
					{
						LogTask._instance._logBuffer.Enqueue(data);
					}
				}
			}
			catch (OutOfMemoryException)
			{
				LogTask._instance.ErrorStatus = ErrorType.MemoryOver;
				// TODO: ログファイルへの出力
			}
			catch (Exception)
			{
				LogTask._instance.ErrorStatus = ErrorType.OtherError;
				// TODO: ログファイルへの出力
			}
		}

		/// <summary>タスクのメインループ</summary>
		private void Loop()
		{
			object data;

			try
			{
				while (true)
				{
					if (ErrorStatus != ErrorType.NoError)
					{
						break;
					}

					if (_enable)
					{
						data = null;

						// Queueに溜まっているログを取得
						lock (_logBufferLock)
						{
							if (_logBuffer.Count != 0)
							{
								data = _logBuffer.Dequeue();
							}
						}

						// ログを送る
						if (data != null)
						{
							if (ProtocolProcessorForEV3.Instance.SendData(data) == false)
							{
								// TODO: ネットワークエラーによる送信失敗
							}
							continue;
						}
					}

					Thread.Sleep(LOOP_INTERVAL);
				}
			}
			catch (InvalidOperationException)
			{
				ErrorStatus = ErrorType.NetworkError;
				// TODO: ログファイルへの出力
			}
			catch (Exception)
			{
				ErrorStatus = ErrorType.OtherError;
				// TODO: ログファイルへの出力
			}
		}
	}
}

