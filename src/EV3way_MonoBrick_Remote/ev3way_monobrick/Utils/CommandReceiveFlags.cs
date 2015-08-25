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

		public CommandReceiveFlags()
		{
		}
	}
}

