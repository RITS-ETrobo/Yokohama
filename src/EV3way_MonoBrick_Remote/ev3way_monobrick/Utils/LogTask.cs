using System;

namespace ETRobocon.Utils
{
	/// <summary>ログ機能(送信)</summary>
	public class LogTask
	{
		public LogTask ()
		{
		}


		/// <summary>ログ送信タスクを開始する</summary>
		/// <remarks>通信確立も行うので, 接続待ち状態となる.</remarks>
		public static void Run()
		{
		}

		/// <summary>ログをリモートで送信する</summary>
		/// <remarks>ログは一旦キューに積まれ, 非同期で送信される.</remarks>
		/// <param name="data">送るデータ</param>
		public static void LogRemote(object data)
		{
		}
	}
}

