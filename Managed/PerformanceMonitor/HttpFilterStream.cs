// File:		HttpFilterStream.cs
// Description:	Template class for creating functional HttpFilter stream classes
//  
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


using System;
using System.IO;

namespace HttpFilter
{
	// Class:		HttpFilterStream 
	// Description:	Base abstract class for an HttpFilter stream class that makes it easier to understand
	//				the restrictions of an HttpFilter stream and create one.
	public abstract class HttpFilterStream : Stream
	{
		protected Stream BaseStream { get; set; }
		protected bool Closed { get; set; } = false;

		// This is an output stream only
		public override bool CanRead { get { return false; } }
		public override bool CanWrite { get { return !Closed; } }

		// No seeking in an HttpFilter
		public override bool CanSeek { get { return false; } }

		// HttpFilter does not support length
		public override long Length => throw new NotSupportedException();

		// HttpFilter does not support position
		public override long Position { get => throw new NotSupportedException(); set => throw new NotSupportedException(); }

		// constructor - holds reference to underlying stream
		protected HttpFilterStream(Stream baseStream)
		{
			BaseStream = baseStream;
		}

		public override void Close()
		{
			Closed = true;
			BaseStream.Close();
		}

		public override void Flush()
		{
			BaseStream.Flush();
		}

		// HttpFilter does not support reads
		public override int Read(byte[] buffer, int offset, int count)
		{
			throw new NotSupportedException();
		}

		// HttpFilter does not support seek
		public override long Seek(long offset, SeekOrigin origin)
		{
			throw new NotSupportedException();
		}

		// HttpFilter does not support SetLength
		public override void SetLength(long value)
		{
			throw new NotSupportedException();
		}


	}
}
