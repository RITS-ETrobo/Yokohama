using System;

namespace ETTobocon.EV3
{
	public class Location
	{
		public double x;
		public double y;

		public Location ()
		{
			x = 0F;
			y = 0F;
		}

		public Location (double prm_x, double prm_y){
			x = prm_x;
			y = prm_y;
		}


		public void setX ( double prm_x){
			x = prm_x;
		}

		public void setY ( double prm_y){
			y = prm_y;
		}

		public double getX ( ){
			return x;
		}

		public double getY ( ){
			return y;
		}

	}
}

