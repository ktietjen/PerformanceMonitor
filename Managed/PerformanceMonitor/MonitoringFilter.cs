// File:		MonitoringFilter.cs
// Description:	HttpFilter stream class to monitor the size of a http response
//  
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information. 

using System;
using System.IO;
using HttpFilter;

namespace HttpModule
{
	// Class:		MonitoringFilter 
	// Description:	This class implements an HttpFilter stream that monitors the size of the http response.
	public class MonitoringFilter : HttpFilterStream
	{
		private long length = 0;

		// Length of the http response
		public override long Length { get { return length; } }
		// Constructor
		public MonitoringFilter(Stream baseStream) : base(baseStream) {}

		// overriden method to count the size of the http response as it is written out
		public override void Write(byte[] buffer, int offset, int count)
		{
			if (Closed)
				throw new ObjectDisposedException("MonitoringFilter");

			BaseStream.Write(buffer, offset, count);

			length += count;
		}
	}
}
