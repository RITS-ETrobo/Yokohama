using System;
using System.Net.Sockets;	// for NetworkStream

namespace ETRobocon.Utils
{
	/// <summary>
	/// PacketDataTypeの拡張メソッド用のクラス.
	/// </summary>
	public static class PacketDataTypeExt
	{
		/// <summary>サイズ取得用</summary>
		static readonly byte[] _sizes = new byte[] {
			0,	// sizes[0] is dummy.
			sizeof(bool),
			sizeof(sbyte),
			sizeof(short),
			sizeof(int),
			sizeof(long),
			sizeof(byte),
			sizeof(ushort),
			sizeof(uint),
			sizeof(ulong),
			sizeof(decimal),
			sizeof(char),
			sizeof(float),
			sizeof(double)
		};

		/// <summary><see cref="PacketDataType"/>のサイズを得る</summary>
		/// <returns>そのデータのサイズ(バイト)</returns>
		/// <param name="packetDataType">
		/// サイズを得たい型.
		/// <see cref="PacketDataType"/>のうち, Invalid, String, NumOfType以外が指定できる.
		/// </param>
		/// <exception cref="System.ArgumentOutOfRangeException">引数が不正.</exception>
		public static byte GetSize(this PacketDataType packetDataType)
		{
			byte typeNum = (byte)packetDataType;
			if (typeNum <= 0 || typeNum >= _sizes.Length) {
				throw new ArgumentOutOfRangeException("packetDataType", "This type is not supported.");
			}

			return _sizes[typeNum];
		}
	}

	/// <summary>送受信するデータの種別</summary>
	/// <remarks>パケットの1byte目にそのまま用いることを想定している.</remarks>
	public enum PacketDataType : byte
	{
		Invalid,	///< 使用不可(不正な型)
		Boolean,
		SByte,
		Short,
		Int,
		Long,
		Byte,
		UShort,
		UInt,
		ULong,
		Decimal,
		Char,
		Float,
		Double,
		String,

		NumOfType	///< データの種類数+1を表す
	};

	/// <summary>EV3-PC間通信プロトコルに従って, EV3-PCの通信を行うためのクラス.</summary>
	/// <remarks>
	/// <para>シングルトンクラスとして利用する. 使用者はEV3, PCそれぞれで一つずつインスタンスを生成する.</para>
	/// <para>EV3-PC間通信プロトコルについては, docs/design/misc/protocol.mdを参照のこと.</para>
	/// </remarks>
	public class ProtocolProcessor
	{
		/// <summary>ProtocolProcessorクラスのインスタンスのフィールド</summary>
		/// <remarks>
		/// C# 6.0 で利用可能な, 自動実装プロパティの初期化が, PC側のRemoteConsoleプロジェクトでビルドできなかった.
		/// C# 6.0 のコードは書かないほうがよさそう??
		/// </remarks>
		protected static ProtocolProcessor _instance = null;

		/// <summary>ProtocolProcessorクラスのインスタンス</summary>
		/// <value>
		/// プログラム上で一つだけ存在するProtocolProcessorクラスのインスタンス.
		/// まだ通信確立済みのインスタンスを生成していないときは<c>null</c>.
		/// </value>
		/// <seealso cref="ProtocolProcessorForEV3.Connect"/>
		/// <seealso cref="ProtocolProcessorForPC.Connect"/>
		public static ProtocolProcessor Instance {
			get { return _instance; }
			private set { _instance = value; }
		}

		/// <summary><see cref="Instance"/>プロパティを排他制御するためのロック</summary>
		/// <remarks>複数タスクが同時にインスタンス生成をするのを防ぐため.</remarks>
		protected static object _instanceLock = new object();

		/// <summary>通信用ストリーム</summary>
		/// <remarks>通信未確立のときは<c>null</c>となる.</remarks>
		protected NetworkStream _stream = null;

		/// <summary><see cref="_stream"/>フィールドを排他制御するためのロック</summary>
		/// <remarks>複数タスクが同時に通信しようとするのを防ぐため.</remarks>
		protected object _streamLock = new object();

		///	通信に用いるポート番号
		protected const int SOCKET_PORT = 7360;

		/// <summary>EV3のIPアドレス</summary>
		protected const string IP_ADDRESS = "10.0.1.1";

		/// <summary><see cref="PacketDataType"/> から <see cref="System.Type"/> への変換用</summary>
		/// <remarks>
		/// <see cref="PacketDataType"/>をインデックスに用いることを想定しているが,
		/// <see cref="PacketDataType.Invalid"/>にあたる<see cref="System.Type"/>が無いことに注意.
		/// (対応が<see cref="PacketDataType"/>と1ずつずれる.)
		/// </remarks>
		private readonly static Type[] types = new Type[(byte)PacketDataType.NumOfType - 1] {
			typeof(bool),
			typeof(sbyte),
			typeof(short),
			typeof(int),
			typeof(long),
			typeof(byte),
			typeof(ushort),
			typeof(uint),
			typeof(ulong),
			typeof(decimal),
			typeof(char),
			typeof(float),
			typeof(double),
			typeof(string)
		};

		/// <summary>コンストラクタ</summary>
		/// <remarks>処理は何も行わないが, アクセスレベルを設定するために定義する.</remarks>
		protected ProtocolProcessor() { }

		/// <summary>デストラクタ</summary>
		/// <remarks>
		/// プログラムが終了するときに一度だけ呼ばれる.
		/// プログラマが明示的に呼ぶことは想定していない.
		/// </remarks>
		~ProtocolProcessor()
		{
			// ソケットを閉じる
			if (_stream != null) {
				_stream.Close();
				_stream = null;
			}
		}

		/// <summary>通信相手へデータを送る</summary>
		/// <param name="data">送るデータ</param>
		/// <returns>正常に送信できたら<c>true</c>, それ以外は<c>false</c>.</returns>
		/// <exception cref="System.InvalidOperationException">未接続状態でメソッドが呼ばれた.</exception>
		/// <exception cref="System.ArgumentException"><paramref name="data"/>がサイズ0のArray型である.</exception>
		/// <exception cref="System.ArgumentNullException"><paramref name="data"/>が<c>null</c>,
		/// または<paramref name="data"/>が<c>null</c>を要素に持つ配列の場合.</exception>
		/// <exception cref="System.ArgumentOutOfRangeException"><paramref name="data"/>の型が, 送信できない型の場合.</exception>
		/// <exception cref="System.Text.EncoderFallbackException"><paramref name="data"/>がASCIIエンコードできない文字列の場合.</exception>
		public bool SendData(object data)
		{
			if (_stream == null) {
				throw new InvalidOperationException("The connection is not established.\n");
			}

			PacketDataType packetDataType;
			byte packetDataCount;
			byte[] packetData;

			// パケットを作成するのに必要な
			// データの種別, データの個数, データのbyte表現を求める
			if (data is Array) {
				Array arrayData = (Array)data;
				packetDataType = GetPacketDataType(arrayData.GetType().GetElementType());
				packetDataCount = (byte)arrayData.Length;
				if (packetDataCount == 0) {
					throw new ArgumentException("The Array length is 0.", "data");
				}
				packetData = new byte[packetDataCount * packetDataType.GetSize()];
				for (int i = 0; i < packetDataCount; i++) {
					PacketDataConverter.ConvertToPacketData(arrayData.GetValue(i), packetDataType).CopyTo(packetData, i * packetDataType.GetSize());
				}
			}
			else if (data is string) {
				packetDataType = PacketDataType.String;
				packetData = PacketDataConverter.ConvertToPacketData(data, packetDataType);
				packetDataCount = (byte)packetData.Length;
			}
			else {
				packetDataType = GetPacketDataType(data.GetType());
				packetDataCount = 1;
				packetData = PacketDataConverter.ConvertToPacketData(data, packetDataType);
			}

			// パケットを作成する
			byte[] packet = new byte[packetData.Length + 2];
			packet[0] = (byte)packetDataType;
			packet[1] = packetDataCount;
			packetData.CopyTo(packet, 2);

			// パケットを送る
			return SendPacketData(packet);
		}

		/// <summary>パケット(byte配列)を送信する</summary>
		/// <param name="packet">送るパケット(byte配列).</param>
		/// <returns>正常に送信できたら<c>true</c>, それ以外の場合は<c>false</c>.</returns>
		private bool SendPacketData(byte[] packet)
		{
			try {
				lock (_streamLock) {
					_stream.Write (packet, 0, packet.Length);
				}
			}
			catch (System.IO.IOException) {
				return false;
			}
			catch (System.ObjectDisposedException) {
				return false;
			}

			return true;
		}

		/// <summary>通信相手からデータを受け取る.</summary>
		/// <remarks>受け取るデータが無かった場合でも待ち状態とならず, メソッドは終了する.</remarks>
		/// <returns>
		/// データを受信できた場合は<c>true</c>, 
		/// 受け取るデータが無かった場合は<c>false</c>.
		/// </returns>
		/// <param name="data">
		/// 受信したパケットの内容を表す<c>object</c>.
		/// ストリームに受信パケットが無かった場合は<c>null</c>となる.
		/// </param>
		/// <exception cref="System.InvalidOperationException">未接続状態でメソッドが呼ばれた.</exception>
		public bool ReceiveData(out object data)
		{
			if (_stream == null) {
				throw new InvalidOperationException("The connection is not established.\n");
			}

			bool ret = false;
			data = null;

			lock (_streamLock) {
				if (_stream.DataAvailable) {

					PacketDataType packetDataType;
					byte packetDataCount;
					byte[] packetData;

					byte[] buffer = new byte[2];
					_stream.Read(buffer, 0, 2);

					// データの種別
					packetDataType = (PacketDataType)buffer[0];

					// データの個数
					packetDataCount = buffer[1];

					// データのbyte表現
					if (packetDataType == PacketDataType.String) {
						packetData = new byte[packetDataCount];
					}
					else {
						packetData = new byte[packetDataCount * packetDataType.GetSize()];
					}
					_stream.Read(packetData, 0, packetData.Length);

					// 読み取ったデータを復元
					data = PacketDataConverter.ConvertFromPacketData(packetData, packetDataCount, packetDataType);

					ret = true;
				}
			}

			return ret;
		}

		/// <summary>
		/// System.TypeからPacketDataTypeへの変換を行う.
		/// </summary>
		/// <returns>
		/// 変換された<see cref="PacketDataType"/>.
		/// 変換できなかったときは<see cref="PacketDataType.Invalid"/>. 
		/// </returns>
		/// <param name="type">変換したい<see cref="System.Type"/>インスタンス.</param>
		private static PacketDataType GetPacketDataType(Type type)
		{
			for (byte i = 0; i < types.Length; i++) {
				if (type.Equals (types [i])) {
					return (PacketDataType)(i + 1);
				}
			}
			return PacketDataType.Invalid;
		}
	}
}
