using System;

namespace ETRobocon.Utils
{
	/// <summary>コマンドのID</summary>
	/// <remarks>EV3, PC両者でIDを統一し, 両者の疎通を図る.</remarks>
	public enum CommandID
	{
		Invalid = -1,	///< 不正なコマンド
		Run,	///< 走行開始
		Stop,	///< 走行停止
		Log,	///< ログ送信有効/無効

		NumOfCommand
	}
}
