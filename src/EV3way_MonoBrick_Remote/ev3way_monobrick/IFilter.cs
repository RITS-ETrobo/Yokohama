using System;
using System.Collections.Generic;
using System.Collections;

namespace ETRobocon.EV3
{
	public interface IFilter
	{
		sbyte Filter (sbyte param); 
	}

	/// <summary>
	/// 移動平均フィルタ
	/// </summary>
	public class MovingAverageFilter : IFilter
	{
		RingList<sbyte> buf;

		public MovingAverageFilter(int bufSize)
		{
			buf = new RingList<sbyte> (bufSize);
		}

		public sbyte Filter( sbyte param )
		{
			buf.Add (param);
			int sum = 0;
			foreach (sbyte s in buf)
			{
				sum += s;
			}

			return (sbyte)(sum / buf.BufUsed);
		}
	}

	public class RingList<Template> : IEnumerable<Template>
	{
		Template[] buf;
		int bufSize;
		int index;
		public int BufUsed
		{ 
			get;
			protected set;
		}
		public RingList( int bufsize )
		{
			bufSize = bufsize;
			buf = new Template[bufSize];
		}

		public void Add( Template data )
		{
			buf [index] = data;
			index = (index + 1) % bufSize;
			if (BufUsed < bufSize)
			{
				BufUsed++;
			}
		}

		public IEnumerator<Template> GetEnumerator()
		{
			for (int i = 0, j = index == 0 ? BufUsed - 1 : (index - 1) % BufUsed ;
				i < BufUsed;
				i++, j = (j + 1) % BufUsed)
			{
				
				yield return buf [j];
			}
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return this.GetEnumerator ();
		}
	}
}

