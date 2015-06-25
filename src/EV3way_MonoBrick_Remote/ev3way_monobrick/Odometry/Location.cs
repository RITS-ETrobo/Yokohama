using System;

namespace ETRobocon.Odometry
{
	/// <summary>
	/// 位置座標(x,y)を表す構造体
	///  - ロボットの位置を表すために使用する
	/// </summary>
	public struct Location
	{
		/// <summary>
		/// 位置座標x
		/// </summary>
		private double _x;

		/// <summary>
		/// 位置座標xのset/getを行うproperty
		/// </summary>
		/// <value>位置座標x.</value>
		public double X{
			set{
				this._x = value;
			}
			get{
				return _x;
			}
		}
			
		/// <summary>
		/// 位置座標y
		/// </summary>
		private double _y;

		/// <summary>
		/// 位置座標yのset/getを行うproperty
		/// </summary>
		/// <value>位置座標y</value>
		public double Y{
			set{
				this._y = value;
			}
			get{
				return _y;
			}
		}

		/// <summary>
		/// 位置座標(x,y)を表す構造体のコンストラクタ
		/// <see cref="ETRobocon.EV3.Location"/>
		/// </summary>
		/// <param name="x">位置座標x.</param>
		/// <param name="y">位置座標y.</param>
		public Location (double x, double y){
			_x = x;
			_y = y;
		}
	}
}

