// File:		PerformanceMonitor.cs
// Description:	.NET managed HTTP Module to 
//					1.) Measure the total time spent processing the request.
//					2.) Measure the time spent by only the IHttpHandler for the request.
//					3.) Measure the size of the response body in bytes.
//						Calculate the minimum, average, and maximum responses seen so far.
//					4.)	Inject measurements and calculation results into response.
//  
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 

using System;
using System.Web;
using System.Diagnostics;

namespace HttpModule
{
	// Class:		PerformanceMonitor 
	// Description:	This class implements the IHttpModule methods needed to benchmark request times and sizes
	//				and inject the benchmark report into the response.
	// Caveats:		This HttpModule is .NET managed code and subject to all the usual security restrictions of
	//				.NET managed code.  This means that the class is loaded and corresponds to a specific AppDomain
	//				and the values apply to that AppDomain and no other.  If you have a site or suite of applications
	//				that run in different AppDomains then their benchmarks will be seperate and pertain to their 
	//				corresponding AppDomains.  This can be overcome or at least worked around by marshaling across
	//				AppDomains, but this comes with a significant performance cost which defeats the purpose of 
	//				bechmarking software.
	public class PerformanceMonitor : IHttpModule
	{
		public PerformanceMonitor() { }

		public String ModuleName
		{
			get { return "PerformanceMonitor"; }
		}

		public long RequestCounter { get; set; } = 0;
		public long RequestSizeTally { get; set; } = 0;
		public long RequestSizeMinimum { get; set; } = -1;
		public long RequestSizeMaximum { get; set; } = 0;
		public void Dispose() { }

		#region IHttpModule Members
		// Initialize and register for events
		public void Init(HttpApplication app)
		{
			//app.BeginRequest += (new EventHandler(this.HandleBeginRequest));
			app.BeginRequest += (s, e) => HandleBeginRequest(new HttpContextWrapper(app.Context));
			app.EndRequest += (s, e) => HandleEndRequest(new HttpContextWrapper(app.Context));
			app.PreRequestHandlerExecute += (s, e) => HandlePreRequestHandler(new HttpContextWrapper(app.Context));
			app.PostRequestHandlerExecute += (s, e) => HandlePostRequestHandler(new HttpContextWrapper(app.Context));
		}

		// Method:		HandleBeginRequest
		// Description: Handles BeginRequest events to start benchmarking the request time.
		//private void HandleBeginRequest(Object source, EventArgs args)
		public void HandleBeginRequest(HttpContextBase context)
		{
			// Setup timer for http request, cache it across requests an start it
			Stopwatch requestTimer = new Stopwatch();
			context.Items["request_timer"] = requestTimer;
			requestTimer.Start();

			// insert http filter to monitor request size
			context.Response.Filter = new MonitoringFilter(context.Response.Filter);
		}

		// Method:		HandleEndRequest
		// Description: Handles EndRequest events to finish benchmarking the request time, incement the response counter
		//				and inject benchmark results.
		public void HandleEndRequest(HttpContextBase context)
		{
			// Get and stop request benchmark timer
			Stopwatch requestTimer = (Stopwatch)context.Items["request_timer"];
			requestTimer.Stop();

			// Get handler bemchmark timer
			Stopwatch handlerTimer = (Stopwatch)context.Items["handler_timer"];

			// Count request
			RequestCounter++;

			// Add current request size to total request size seen
			RequestSizeTally += context.Response.Filter.Length;

			// Update minimum request size
			if (context.Response.Filter.Length > 0 && (context.Response.Filter.Length < RequestSizeMinimum || RequestSizeMinimum == -1))
				RequestSizeMinimum = context.Response.Filter.Length;

			// Update maximum request sie
			if (context.Response.Filter.Length > RequestSizeMaximum)
				RequestSizeMaximum = context.Response.Filter.Length;

			// Inject report if this is a text/html response
			if (context.Response.ContentType == "text/html" && context.Response.StatusCode == 200)
			{
				string result = string.Format("<hr/>Response Size (bytes): current {{{0}}} - minimum {{{1}}} - average {{{2}}} - maximum {{{3}}}",
					context.Response.Filter.Length, RequestSizeMinimum, RequestSizeTally / RequestCounter,
					RequestSizeMaximum);

				// inject response size benchmarks
				context.Response.Write(result);

				// get the request time in seconds
				double requestSeconds = (double)requestTimer.ElapsedTicks / Stopwatch.Frequency;

				// for some request the handler time is not available
				result = string.Format("<br>Time: Request {{{0:F6} s}}", requestSeconds);
				if (handlerTimer != null)
				{
					double handlerSeconds = (double)handlerTimer.ElapsedTicks / Stopwatch.Frequency;
					result = result + string.Format(" - Handler {{{0:F6} s}}", handlerSeconds);
				}

				// inject request time benchmarks
				context.Response.Write(result);
			}
		}

		// Method:		HandlePreRequestHandler
		// Description: Handles PreRequestHandler events start handler benchmark timer
		public void HandlePreRequestHandler(HttpContextBase context)
		{
			// Setup timer for http handler, cache it across requests an start it
			Stopwatch httpHandlerTimer = new Stopwatch();
			context.Items["handler_timer"] = httpHandlerTimer;
			httpHandlerTimer.Start();
		}

		// Method:		HandlePostRequestHandler
		// Description: Handles PostRequestHandler events to finish benchmarking the handler time
		public void HandlePostRequestHandler(HttpContextBase context)
		{
			// Get and stop request timer
			Stopwatch httpHandlerTimer = (Stopwatch)context.Items["handler_timer"];
			httpHandlerTimer.Stop();
		}

		#endregion
	}
}
