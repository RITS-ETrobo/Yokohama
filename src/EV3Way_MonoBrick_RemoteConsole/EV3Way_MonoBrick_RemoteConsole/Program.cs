using System;
using System.Text;
using System.Threading;

using EV3Way_MonoBrick_RemoteConsole.Utils;

namespace EV3Way_MonoBrick_RemoteConsole
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			try {
				CommandTask.Run();
				LogTask.Run();
			} catch (Exception e) {
				Console.Out.WriteLine("caught an exception: {0}", e.Message);
			}
		}
	}
}
