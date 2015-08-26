using System;

namespace ETRobocon.Utils
{
	public class CommandReceiveFlags
	{
        /// <summary>Instanceのフィールド</summary>
        private static CommandReceiveFlags _instance = new CommandReceiveFlags();
        /// <summary>インスタンスへの参照</summary>
        public static CommandReceiveFlags Instance
        {
            get
            {
                return _instance;
            }
        }

        /// <summary>コマンド受信フラグ</summary>
        /// <remarks>各要素は, <see cref="CommandID"/>の定義順と一致する.</remarks>
        private bool[] _flags;

		public CommandReceiveFlags()
		{
            _flags = new bool[(int)CommandID.NumOfCommand];
            // 一応, 明示的に初期化
            for (int i = 0; i < _flags.Length; i++)
            {
                _flags[i] = false;
            }
		}

        /// <summary>コマンドを受信した</summary>
        /// <param name="command">受信したコマンドのID</param>
        public void ReceiveCommand(CommandID command)
        {
            _flags[(int)command] = true;
        }

        /// <summary>コマンドを受信したか確認</summary>
        /// <remarks>確認をすると, そのコマンドの受信状況は, "未受信"にリセットされる.</remarks>
        /// <param name="command">確認するコマンドのID</param>
        /// <returns>コマンドを受信していた場合はtrue, 未受信の場合はfalse</returns>
        public bool CheckCommandReceived(CommandID command)
        {
            bool ret = _flags[(int)command];
            _flags[(int)command] = false;
            return ret;
        }
	}
}

