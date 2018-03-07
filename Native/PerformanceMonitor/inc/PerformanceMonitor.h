// File:		PerformanceMonitor.h
// Description:	Native HTTP Module to 
//					1.) Measure the total time spent processing the request.
//					2.) Measure the time spent by only the IHttpHandler for the request.
//					3.) Measure the size of the response body in bytes.
//						Calculate the minimum, average, and maximum responses seen so far.
//					4.)	Inject measurements and calculation results into response.
//
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 


#ifndef __PERFORMANCE_MONITOR_HTTP_MODULE_H__
#define __PERFORMANCE_MONITOR_HTTP_MODULE_H__

#include <stdio.h>
#include <climits>
#include "HighResolutionTimer.h"

// Class:		PerformanceMonitorHttpModule 
// Description:	This class implements the CHTTPModule methods needed to benchmark request times and sizes
//				and inject the benchmark report into the response.
class PerformanceMonitorHttpModule : public CHttpModule
{
private:
	static HANDLE		g_mutex;		// to protect reading and writing static variables between threads (ie. different http requests/responses)
	HighResolutionTimer _requestTimer;
	HighResolutionTimer _handlerTimer;

	long long			_responseSize = 0;
	static long long	g_responseCount;
	static long long	g_totalResponseSize;
	static long long	g_minimumResponseSize;
	static long long	g_maximumResponseSize;

	const char*			_benchmarkReportFormat = "<hr / >Response Size(bytes) : current { %lld } - minimum { %lld } - average { %lld } - maximum { %lld }<br>Time: Handler { %4.6f s } - Request { %4.6f s }";

	bool				_endRequestCalled = false;
	bool				_sendResponseCalled = false;

	// Constructor
public:
	PerformanceMonitorHttpModule()
	{}

	~PerformanceMonitorHttpModule()
	{}


	// CHttpModule methods (events)
public:
	REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
	REQUEST_NOTIFICATION_STATUS OnEndRequest(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider);

	REQUEST_NOTIFICATION_STATUS OnPostPreExecuteRequestHandler(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider);
	REQUEST_NOTIFICATION_STATUS OnExecuteRequestHandler(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider);
	REQUEST_NOTIFICATION_STATUS OnPostExecuteRequestHandler(IN IHttpContext * pHttpContext, IN IHttpEventProvider * pProvider);

	REQUEST_NOTIFICATION_STATUS OnSendResponse(IN IHttpContext* pHttpContext, IN ISendResponseProvider* pProvider);

	// Class helper methods
private:
	long long GetHeaderSize(HTTP_RESPONSE_HEADERS headers);
	long long GetChunkSize(HTTP_DATA_CHUNK chunk);
	void UpdateRequestSizeBenchmarks();
	void WriteBenchmarksToResponse(IN IHttpContext * pHttpContext);
};

#endif
