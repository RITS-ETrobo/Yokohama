using System;
using System.Net.Sockets;	// for Socket, AddressFamily, SocketType, ProtocolType, NetworkStream

namespace ETRobocon.Utils
{
	/// <summary>
	/// EV3-PC間通信プロトコルに従って, EV3-PCの通信を行うための機能のうち, PC側を担当するクラス.
	/// </summary>
	public class ProtocolProcessorForPC : ProtocolProcessor
	{
		/// <summary>EV3との接続の最大試行回数</summary>
		/// <remarks>1回の試行は約1秒</remarks>
		private const int CONNECTION_RETRY_COUNT = 60;

		/// <summary>EV3との通信確立済みのインスタンスを生成するコンストラクタ</summary>
		private ProtocolProcessorForPC()
		{
			_stream = null;
			// EV3との接続
			int retry = CONNECTION_RETRY_COUNT;
			while (retry > 0) {
				try {
					// 指定されたサーバに接続
					Socket	sock = new Socket (
						AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
					sock.Connect(IP_ADDRESS, SOCKET_PORT);

					_stream = new NetworkStream (sock, true);

					return;
				} catch (Exception e) {
					retry--;
				}
			}
		}

		/// <summary>EV3と通信を確立する</summary>
		/// <remarks>通信が既に確立していたら何もしない.</remarks>
		public static void Connect()
		{
			lock (_instanceLock) {	// 複数のInstanceを生成してしまうことのないよう
				if (_instance == null) {
					_instance = new ProtocolProcessorForPC();
				}
			}
		}
	}
}

