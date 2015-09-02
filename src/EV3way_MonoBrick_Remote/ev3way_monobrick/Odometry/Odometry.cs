using System;

namespace ETRobocon.Odometry
{
	/// <summary>
	/// 自己位置推定を行うクラス
	/// </summary>
	public class Odometry
	{
		/// <summary>
		/// 車輪の半径[mm](refs #65)
		/// </summary>
		private const double WHEEL_RADIUS_MM = 41.0;

		/// <summary>
		/// 車軸の長さ[mm](refs #65)
		/// </summary>
		private const double AXLE_LENGTH_MM = 160.0;

		/// <summary>
		/// ロボットが現在いる位置
		/// </summary>
		private Location _curLocation = new Location(0.0 , 0.0);

		/// <summary>
		/// ロボットが前回いた位置
		/// </summary>
		private Location _prevLocation = new Location (0.0, 0.0);

		/// <summary>
		/// 現在の右モーターのエンコーダー値[度]
		/// </summary>
		private int _curRightEncoderDEG = 0;

		/// <summary>
		/// 現在の左モーターのエンコーダー値[度]
		/// </summary>
		private int _curLeftEncoderDEG = 0; 

		/// <summary>
		/// 右エンコーダー値の前回との差分[radian]
		/// </summary>
		private double _diffRightEncoderRAD = 0.0;

		/// <summary>
		/// 左エンコーダー値の前回との差分[radian]
		/// </summary>
		private double _diffLeftEncoderRAD = 0.0;

		/// <summary>
		/// ロボットの累積走行距離[mm]
		/// </summary>
		private double _totalMoveDistanceMM = 0.0;

		/// <summary>
		/// 現在のロボットの旋回角度[radian]
		/// </summary>
		private double _curThetaRAD = 0.0;

		/// <summary>
		/// ロック用オブジェクト
		/// </summary>
		private readonly Object LOCK_OBJ = new Object();

		/// <summary>
		/// 自己位置推定クラスのコンストラクタ
		/// <see cref="ETRobocon.Odometry.Odometry"/>
		/// </summary>
		public Odometry ()
		{
		}

		/// <summary>
		/// 自己位置推定を実施する.
		/// </summary>
		/// <param name="leftTachoCount">左モーターのMotor.getTachoCount()の値[度].</param>
		/// <param name="rightTachoCount">右モーターのMotor.getTachoCount()の値[度].</param>
		public void update(int leftTachoCount , int rightTachoCount){

			//前回の結果をローカル変数に格納
			//				Location prev_location = _curLocation;
			_prevLocation = _curLocation;
			int prev_right_encoder_deg = _curRightEncoderDEG;
			int prev_left_encoder_deg = _curLeftEncoderDEG;
			double prev_theta_rad = _curThetaRAD;

			//現在のエンコーダー値[度]の更新
			_curRightEncoderDEG = rightTachoCount;
			_curLeftEncoderDEG = leftTachoCount;

			//エンコーダー値の差分[radian]の算出
			_diffRightEncoderRAD = degreeToRadian( _curRightEncoderDEG - prev_right_encoder_deg);
			_diffLeftEncoderRAD = degreeToRadian( _curLeftEncoderDEG - prev_left_encoder_deg);

			//走行距離[mm]の算出
			double delta_right_move_distance_mm = WHEEL_RADIUS_MM * _diffRightEncoderRAD; //右車輪走行距離増加分
			double delta_left_move_distance_mm = WHEEL_RADIUS_MM * _diffLeftEncoderRAD; //左車輪走行距離増加分
			double delta_move_distance_mm = (delta_right_move_distance_mm + delta_left_move_distance_mm) / 2.0; //ロボット全体の走行距離増加分

			//ロボットの旋回角度増加分[radian]の算出
			double delta_theta_rad = (delta_right_move_distance_mm - delta_left_move_distance_mm) / AXLE_LENGTH_MM;

			//ロボットの現在値を求めるための値を算出
			double trigono_func_arg = prev_theta_rad + delta_theta_rad / 2.0;
			double coefficient = delta_move_distance_mm * sinc_approx( delta_theta_rad / 2.0); 

			lock(LOCK_OBJ){// 累積走行距離と現在地を更新中にアクセスできないようにするため
				//ロボットの現在地の更新
				_curLocation.X = _prevLocation.X + coefficient * Math.Cos (trigono_func_arg);
				_curLocation.Y = _prevLocation.Y + coefficient * Math.Sin (trigono_func_arg);
//				_curLocation = new Location (
//					prev_location.X + coefficient * Math.Cos (trigono_func_arg),
//					prev_location.Y + coefficient * Math.Sin (trigono_func_arg)
//				);

				//ロボットの旋回角度[radian]の更新
				_curThetaRAD = prev_theta_rad + delta_theta_rad;

				//累積走行距離[mm]に加算
				_totalMoveDistanceMM += delta_move_distance_mm; 
			}
		}

		/// <summary>
		/// 度数法[度]から弧度法[radian]へ変換する.
		/// </summary>
		/// <returns>弧度法に直した値[radian].</returns>
		/// <param name="degree">度数法の値[度].</param>
		private double degreeToRadian (int degree){
			return degree * Math.PI / 180.0;
		}

		/// <summary>
		/// sinc関数の近似解をテイラー展開で求める.
		/// </summary>
		/// <returns>The approx.</returns>
		/// <param name="rad">sinc関数の引数[radian].</param>
		private double sinc_approx( double rad){

			double ret = 1.0;
			double rad_squared = Math.Pow (rad, 2);

			ret -= rad_squared / (3 * 2 * 1);
			ret += Math.Pow (rad_squared, 2) / (5 * 4 * 3 * 2 * 1);
			//ret -= Math.Pow (rad_squared, 3) / (7 * 6 * 5 * 4 * 3 * 2 * 1);
			//ret += Math.Pow (rad_squared, 4) / (9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
			//ret -= Math.Pow (rad_squared, 5) / (11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
			return ret;
		}

		/// <summary>
		/// ロボットの現在値を取得する.
		/// </summary>
		/// <returns>ロボットの現在値.</returns>
		public Location CurLocation{
			get{
				lock (LOCK_OBJ) {
					return this._curLocation;
				}
			}
		}

		/// <summary>
		/// ロボットの累積走行距離を取得する.
		/// </summary>
		/// <returns>累積走行距離[mm].</returns>
		public double TotalMoveDistanceMM{
			get{
				lock (LOCK_OBJ) {
					return this._totalMoveDistanceMM;
				}
			}
		}

		/// <summary>
		/// ロボットの進行方向を取得する.
		/// </summary>
		/// <value>ロボットの進行方向[RAD].</value>
		public double CurThetaRAD{
			get{
				lock (LOCK_OBJ) {
					return this._curThetaRAD;
				}
			}
		}
	}
}

