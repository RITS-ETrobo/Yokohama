using System;
using System.Threading;
using System.Threading.Tasks;

using ETRobocon.Utils;
using MonoBrickFirmware.Movement;

namespace ETRobocon.Odometry
{
	/// <summary>
	/// 自己位置推定計算を行うタスク.
	/// </summary>
	public class OdometryTask
	{
		/// <summary>
		/// 左モーターのインスタンス.
		/// </summary>
		private Motor _motorL;

		/// <summary>
		/// 右モーターのインスタンス.
		/// </summary>
		private Motor _motorR;

		/// <summary>
		/// 自己位置推定クラスのインスタンス.
		/// </summary>
		private Odometry _odometry;

		/// <summary>
		/// 自己位置推定計算タスクをループさせるための変数.
		///  - trueでループ中
		///  - falseでループ終了
		/// </summary>
		private bool _isRoop = false;

		/// <summary>
		/// 自己位置推定計算タスクの実施間隔[msec]
		/// 2014年度は120msec毎(4msecごとのループで30回毎)に自己位置推定の計算を実施していた
		/// 2015年度もとりあえず流用するが、別途実機での動作検証必要(refs #115)
		/// </summary>
		const int INTERVAL_TIME = 120;

		/// <summary>
		/// Initializes a new instance of the <see cref="ETRobocon.Odometry.OdometryTask"/> class.
		/// </summary>
		/// <param name="motorL">左モーターのインスタンス.</param>
		/// <param name="motorR">右モーターのインスタンス.</param>
		public OdometryTask (Motor motorL, Motor motorR)
		{
			_motorL = motorL;
			_motorR = motorR;
			_odometry = new Odometry ();
		}

		/// <summary>
		/// 自己位置推定計算タスクの開始.
		/// </summary>
		public void startTask(){
		
			_isRoop = true;

			Task.Factory.StartNew (() => {
			
				while(_isRoop){
					_odometry.update(_motorL.GetTachoCount(),_motorR.GetTachoCount());
					sendOdometryLogTask(_odometry.TotalMoveDistanceMM,_odometry.CurLocation.X,_odometry.CurLocation.Y);
					Thread.Sleep(INTERVAL_TIME);
				}
			});

		//	task.Wait();
		}

		/// <summary>
		/// 自己位置推定計算タスクの終了.
		/// </summary>
		public void endTask(){
			_isRoop = false;
		}

		/// <summary>
		/// 自己位置推定計算タスクで求めたロボットの現在値の取得.
		/// </summary>
		/// <returns>ロボットの現在値.</returns>
		public Location getCurrentLocation(){
			return _odometry.CurLocation;
		}

		/// <summary>
		/// 自己位置推定計算タスクで求めた累積走行距離[mm]の取得.
		/// </summary>
		/// <returns>累積走行距離[mm].</returns>
		public double getTotalMoveDistanceMM(){
			return _odometry.TotalMoveDistanceMM;
		}

		/// <summary>
		/// 自己位置推定のログを送信するタスク
		/// </summary>
		private void sendOdometryLogTask(double distance,double locX,double locY){

			Task.Factory.StartNew (() => {
				ProtocolProcessorForEV3.Instance.SendData(new double[3]{ distance, locX, locY });
			});

		}

	}
}

