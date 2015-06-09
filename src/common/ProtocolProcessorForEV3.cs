using System;
using System.Net;	// for IPAddress
using System.Net.Sockets;	// for Socket, SocketException, TcpListener, NetworkStream

namespace ETRobocon.Utils
{
	/// <summary>
	/// EV3-PC間通信プロトコルに従って, EV3-PCの通信を行うための機能のうち, EV3側を担当するクラス.
	/// </summary>
	public class ProtocolProcessorForEV3 : ProtocolProcessor
	{
		/// <summary>PCとの通信確立済みのインスタンスを生成するコンストラクタ</summary>
		private ProtocolProcessorForEV3()
		{
			// PCとの接続
			IPAddress ipAddr = IPAddress.Parse(IP_ADDRESS);

			var listener = new TcpListener (ipAddr, SOCKET_PORT); 
			listener.Start(); // クライアントからの受信接続要求の待機を開始

			try {
				Socket sock = listener.AcceptSocket(); // 接続要求の受け入れ
				_stream = new NetworkStream(sock, true);
			} catch (SocketException) {
				_stream = null;
			}
			listener.Stop();
		}

		/// <summary>PCと通信を確立する</summary>
		/// <remarks>通信が既に確立していたら何もしない.</remarks>
		public static void Connect()
		{
			lock (_instanceLock) {	// 複数のInstanceを生成してしまうことのないよう
				if (_instance == null) {
					_instance = new ProtocolProcessorForEV3();
				}
			}
		}
	};
}

