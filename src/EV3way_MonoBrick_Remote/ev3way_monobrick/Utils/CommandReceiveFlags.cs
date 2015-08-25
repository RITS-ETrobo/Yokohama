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
	}
}

