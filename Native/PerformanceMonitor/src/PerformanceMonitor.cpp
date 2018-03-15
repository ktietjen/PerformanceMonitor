// File:		 PerformanceMonitorHttpModule.cpp
// Description:	 Native HTTP Module to 
//					1.) Measure the total time spent processing the request.
//					2.) Measure the time spent by only the IHttpHandler for the request.
//					3.) Measure the size of the response body in bytes.
//						Calculate the minimum, average, and maximum responses seen so far.
//					4.)	Inject measurements and calculation results into response.
//  
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 

#include "precomp.h"

// Initialize static member variables
HANDLE		PerformanceMonitorHttpModule::g_mutex = CreateMutex(NULL, FALSE, NULL);
long long	PerformanceMonitorHttpModule::g_responseCount = 0;
long long	PerformanceMonitorHttpModule::g_totalResponseSize = 0;
long long	PerformanceMonitorHttpModule::g_minimumResponseSize = LLONG_MAX;
long long	PerformanceMonitorHttpModule::g_maximumResponseSize = 0;

// Method:		OnOnBeginRequest
// Description: Handles OnBeginRequest events to start benchmarking the request time.
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnBeginRequest(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider)
{
	UNREFERENCED_PARAMETER(pHttpContext);
	UNREFERENCED_PARAMETER(pProvider);

	// start request benchmark timer
	_requestTimer.Start();

	// Return processing to the pipeline.
	return RQ_NOTIFICATION_CONTINUE;
}

// Method:		OnEndRequest
// Description: Handles OnEndRequest events to finish benchmarking the request time, incement the response counter
//				and if opportune, to inject benchmark results.
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnEndRequest(IN IHttpContext * pHttpContext, IN IHttpEventProvider* pProvider)
{
	UNREFERENCED_PARAMETER(pProvider);

	// used to signal OnSendResponse that we have seen the EndRequest event
	_endRequestCalled = true;

	// stop request benchmark timer
	_requestTimer.Stop();

	// guard against multi-threaded updates to static variables
	{
		WaitForSingleObject(g_mutex, INFINITE);

		// count this response
		g_responseCount++;

		// The SendResponse event is non-deterministic and we use it to calculate response size. We need to wait until both
		// EndRequest and SendResponse events are called. If SendResponse has not been called, we will wait until it has
		// been called and then update and write out the benchmarks. We don't know if another SendResponse will be called
		// so we will update and write out the benchmarks when we are guaranteed to be called which is now. This means that
		// the response size may be inaccurate.
		if (_sendResponseCalled)
			WriteBenchmarksToResponse(pHttpContext);

		ReleaseMutex(g_mutex);
	}

	// Return processing to the pipeline.
    return RQ_NOTIFICATION_CONTINUE;
}

// Method:		OnPostPreExecuteRequestHandler
// Description: Handles OnPostPreExecuteRequestHandler events to start the handler benchmark timer. On some systems we
//				do not recieve the OnExecuteRequestHandler event for some reason and this event is then the next best
//				event to start the benchmark timer
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnPostPreExecuteRequestHandler(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider)
{
	UNREFERENCED_PARAMETER(pHttpContext);
	UNREFERENCED_PARAMETER(pProvider);

	// start handler benchmark timer
	_handlerTimer.Start();

	return RQ_NOTIFICATION_CONTINUE;
}

// Method:		OnExecuteRequestHandler
// Description: Handles OnExecuteRequestHandler events to start the handler benchmark timer. On some systems we
//				do not recieve the OnExecuteRequestHandler event for some reason and so we start the timer in the
//				OnPostPreExecuteRequestHandler event and if we recieve OnExecuteRequestHandler event we simply
//				restart the timer to be more accurate.
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnExecuteRequestHandler(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider)
{
	UNREFERENCED_PARAMETER(pHttpContext);
	UNREFERENCED_PARAMETER(pProvider);

	// start or restart handler benchmark timer
	_handlerTimer.Start();

	return RQ_NOTIFICATION_CONTINUE;
}

// Method:		OnPostExecuteRequestHandler
// Description: Handles OnPostExecuteRequestHandler events to stop the handler benchmark timer.
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnPostExecuteRequestHandler(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider)
{
	UNREFERENCED_PARAMETER(pHttpContext);
	UNREFERENCED_PARAMETER(pProvider);

	// stop request benchmark timer
	_handlerTimer.Stop();

	return RQ_NOTIFICATION_CONTINUE;
}

// Method:		OnSendResponse
// Description: Handles OnSendResponse events to calculate the response size and if opportune, inject the benchmark results.
REQUEST_NOTIFICATION_STATUS PerformanceMonitorHttpModule::OnSendResponse(IN IHttpContext * pHttpContext, IN ISendResponseProvider * pProvider)
{
	// used to signal OnEndRequest that we have seen the OnSendResponse event
	_sendResponseCalled = true;

	if (pHttpContext != NULL)
	{
		HTTP_RESPONSE* pHttpRawResponse = pHttpContext->GetResponse()->GetRawHttpResponse();

		if (pHttpContext != NULL)
		{
			// Count the http response status line by recreating it
			const char* httpResponseFormat = "HTTP/%d.%d %d ";
			char httpResponseBuffer[25];

			if (_snprintf_s(httpResponseBuffer, 25, _TRUNCATE, httpResponseFormat, pHttpRawResponse->Version.MajorVersion, pHttpRawResponse->Version.MinorVersion,
				pHttpRawResponse->StatusCode) < 1)
				return RQ_NOTIFICATION_CONTINUE; // abort as something has seriously gone wrong

			_responseSize = strnlen(httpResponseBuffer, 25) + pHttpRawResponse->ReasonLength + 2; // add chars /r, /n

			// add the headers
			_responseSize += GetHeaderSize(pHttpRawResponse->Headers);

			// add the body
			if (pHttpRawResponse->pEntityChunks != NULL)
			{
				for (int index = 0; index < pHttpRawResponse->EntityChunkCount; index++)
				{
					_responseSize += GetChunkSize(pHttpRawResponse->pEntityChunks[index]);
				}
			}
		}

		// SendResponse is non-deterministic and we use it to calculate response size. We need to wait until both EndRequest and SendResponse events are called.
		// If EndRequest has not been called, we will wait until it has been called and then update and write out the benchmarks.
		// We don't know if another SendResponse will be called so we will update and write out the benchmarks when we are guaranteed to be called which is when.
		// EndRequest is called. This means that the response size maybe inaccurate.
		if (_endRequestCalled)
		{
			// guard against multi-threaded updates to static variables
			{
				WaitForSingleObject(g_mutex, INFINITE);

				WriteBenchmarksToResponse(pHttpContext);

				ReleaseMutex(g_mutex);
			}
		}
	}

	return RQ_NOTIFICATION_CONTINUE;
}

// Method:		GetHeaderSize
// Description: Calculates and returns the size of the response headers.
long long PerformanceMonitorHttpModule::GetHeaderSize(HTTP_RESPONSE_HEADERS headers)
{
	long long size = 0;
	int headerNameSize;
	PCSTR headerName;

	// add the accumulative sizes of the unknown type headers
	if ((headers.UnknownHeaderCount > 0) && (headers.pUnknownHeaders != NULL))
	{
		for (int index = 0; index <= headers.UnknownHeaderCount; index++)
		{
			size += headers.pUnknownHeaders[index].NameLength + headers.pUnknownHeaders[index].RawValueLength + 4; // add the :, space, /n,/r chars
		}
	}

	// add the accumuative sizes of the known type headers
	for (int index = 0; index < HttpHeaderResponseMaximum; index++)
	{
		// only count the existing headers
		if (headers.KnownHeaders[index].RawValueLength)
		{
			headerNameSize = 0;

			// get the length of the key
			switch (index)
			{
			case HttpHeaderProxyAuthenticate:		// "Proxy-Authenticate"
				headerNameSize = 18;
				break;
			case HttpHeaderTransferEncoding:		// "Transfer-Encoding"
				headerNameSize = 17;
				break;
			case HttpHeaderContentEncoding:			// "Content-Encoding"
			case HttpHeaderContentLanguage:			// "Content-Language"
			case HttpHeaderContentLocation:			// "Content-Location"
			case HttpHeaderWwwAuthenticate:			// "WWW-Authenticate"
				headerNameSize = 16;
				break;
			case HttpHeaderContentLength:
				headerNameSize = 14;				// "Content-Length"
				break;
			case HttpHeaderContentRange:			// "Content-Range"
			case HttpHeaderLastModified:			// "Last-Modified"
			case HttpHeaderAcceptRanges:			// "Accept-Ranges"
				headerNameSize = 13;
				break;
			case HttpHeaderContentType:				// "Content-Type"
				headerNameSize = 12;
				break;
			case HttpHeaderContentMd5:				// "Content-MD5"
			case HttpHeaderRetryAfter:				// "Retry-After"
				headerNameSize = 11;
				break;
			case HttpHeaderConnection:				// "Connection"
			case HttpHeaderKeepAlive:				// "Keep-Alive"
			case HttpHeaderSetCookie:				// "Set-Cookie"
				headerNameSize = 10;
				break;
			case HttpHeaderTrailer:					// "Trailer"
			case HttpHeaderUpgrade:					// "Upgrade"
			case HttpHeaderWarning:					// "Warning"
			case HttpHeaderExpires:					// "Expires"
			case HttpHeaderLocation:				// "Location"
				headerNameSize = 7;
				break;
			case HttpHeaderPragma:					// "Pragma"
			case HttpHeaderServer:					// "Server"
				headerNameSize = 6;
				break;
			case HttpHeaderAllow:					// "Allow"
				headerNameSize = 5;
				break;
			case HttpHeaderDate:					// "Date"
			case HttpHeaderEtag:					// "ETag"
			case HttpHeaderVary:					// "Vary"
				headerNameSize = 4;
				break;
			case HttpHeaderVia:						// "Via"
			case HttpHeaderAge:						// "Age"
				headerNameSize = 3;
				break;
			}

			// add key, value and formating space of each header pair
			size += headerNameSize + headers.KnownHeaders[index].RawValueLength + 4;  // add the :, space, /n,/r chars
		}
	}

	return size;
}

// Method:		GetChunkSize
// Description: Calculates and return the size of the response chunks
long long PerformanceMonitorHttpModule::GetChunkSize(HTTP_DATA_CHUNK chunk)
{
	long long size = 0;

	switch (chunk.DataChunkType)
	{
	case HttpDataChunkFromMemory:
		size = chunk.FromMemory.BufferLength;
		break;
	case HttpDataChunkFromFileHandle:
		size = chunk.FromFileHandle.ByteRange.Length.QuadPart;
		break;
	case HttpDataChunkFromFragmentCache:
		size = chunk.FromFragmentCache.FragmentNameLength;
		break;
	case HttpDataChunkFromFragmentCacheEx:
		size = chunk.FromFragmentCacheEx.ByteRange.Length.QuadPart;
		break;
	}

	return size;
}

// Method:		UpdateRequestSizeBenchmarks
// Description: Calculates and updates the minimum, maximum and total response sizes.
void PerformanceMonitorHttpModule::UpdateRequestSizeBenchmarks()
{
	if (g_minimumResponseSize > _responseSize)
		g_minimumResponseSize = _responseSize;

	if (g_maximumResponseSize < _responseSize)
		g_maximumResponseSize = _responseSize;

	g_totalResponseSize += _responseSize;
}

// Method:		WriteBenchmarksToResponse
// Description: Writes the benchmark report to the response
// TODO: Update to rewrite the last part of the reponse to make sure that we inject the benchmark
//		 before the closing html tag.  Right now we just inject the benchmark at the end of the response
//		 which produces invalid html, but browsers are forgiving :-)
void PerformanceMonitorHttpModule::WriteBenchmarksToResponse(IN IHttpContext * pHttpContext)
{
	if (pHttpContext != NULL)
	{
		// Update benchmarks in preparation to write the report
		UpdateRequestSizeBenchmarks();

		if (pHttpContext != NULL)
		{
			IHttpResponse* response = pHttpContext->GetResponse();

			if (response != NULL)
			{
				USHORT	headerLength;
				PCSTR	contentTypeHeader = response->GetHeader(HttpHeaderContentType, &headerLength);

				// only write the benchmarks if the response type is text/html
				if (contentTypeHeader != NULL && headerLength >= 9 && (strncmp("text/html", contentTypeHeader, 9) == 0))
				{
					HTTP_DATA_CHUNK* pChunk = (HTTP_DATA_CHUNK*)pHttpContext->AllocateRequestMemory(sizeof(HTTP_DATA_CHUNK));
					if (pChunk != NULL)
					{
						pChunk->DataChunkType = HttpDataChunkFromMemory;
						pChunk->FromMemory.pBuffer = pHttpContext->AllocateRequestMemory(512);
						pChunk->FromMemory.BufferLength = _snprintf_s((char* const)pChunk->FromMemory.pBuffer, 512, _TRUNCATE, _benchmarkReportFormat,
							_responseSize, g_minimumResponseSize, (g_totalResponseSize / g_responseCount), g_maximumResponseSize,
							_handlerTimer.ElapsedSeconds(), _requestTimer.ElapsedSeconds());

						if (pChunk->FromMemory.BufferLength > 0) // only write to response if buffer write successful 
							response->WriteEntityChunkByReference(pChunk, -1);
					}
				}
			}
		}
	}
}