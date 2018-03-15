// File:		PerformanceMonitorUnitTests.cs
// Description:	Unit Tests for the Performance Monitor 


using System;
using System.Web;
using HttpModule;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace PerformanceMonitorUnitTests
{
	// Class:		MockHttpContext 
	// Description:	Defines a mock class for HttpContext object for use in unit tests
	public class MockHttpContext : HttpContextBase
	{
		Dictionary<string, Object> items = new Dictionary<string, object>();
		MockResponseContext response = new MockResponseContext();

		public MockHttpContext()
		{
			response.Filter = new MemoryStream();
			response.ContentType = "text/html";
			response.StatusCode = 200;
		}
		public override IDictionary Items { get { return items; } }
		public override HttpResponseBase Response { get { return response; } }
	}

	// Class:		MockResponseContext 
	// Description:	Defines a mock class for HttpResponse object for use in unit tests
	public class MockResponseContext : HttpResponseBase
	{
		public override Stream Filter { get; set; }
		public override string ContentType { get; set; }
		public override int StatusCode { get; set; }
		public override void Write(string s)
		{
			Filter.Write(Encoding.ASCII.GetBytes(s), 0, s.Length);
		}
	}

	// Class:		PerformanceMonitorUnitTests 
	// Description:	Class that defines unit tests for PerformanceMonitor that cover all
	//				intended functionality and critical failures.
	[TestClass]
	public class PerformanceMonitorUnitTests
	{
		#region HandleBeginRequest_Tests

		// Method:		HandleBeginRequest_Stopwatch_Tests
		// Description: Tests HandleBeginRequest Stopwatch operations
		[TestMethod]
		public void HandleBeginRequest_Stopwatch_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandleBeginRequest(context);

			// Stopwatch tests
			Pre_Stopwatch_Tests(context, "request_timer");
		}

		// Method:		HandleBeginRequest_HttpFilter_Tests
		// Description: Tests HandleBeginRequest HttpFilter operations
		[TestMethod]
		public void HandleBeginRequest_HttpFilter_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandleBeginRequest(context);

			// HttpFilter tests
			Assert.IsNotNull(context.Response.Filter);
			Assert.IsTrue(context.Response.Filter.GetType() == typeof(MonitoringFilter));
		}

		// Method:		HandleBeginRequest_CriticalException_Tests
		// Description: Tests HandleBeginRequest critical failure states
		[TestMethod]
		public void HandleBeginRequest_CriticalException_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();

			// Critical Exception Handling
			pm.HandleBeginRequest(null);
		}

		#endregion

		#region HandleEndRequest_Tests

		// Method:		HandleEndRequest_Stopwatch_Tests
		// Description: Tests HandleEndRequest Stopwatch operations
		[TestMethod]
		public void HandleEndRequest_Stopwatch_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandleBeginRequest(context);
			pm.HandleEndRequest(context);

			// Stopwatch tests
			Post_Stopwatch_Tests(context, "request_timer");
		}

		// Method:		HandleEndRequest_ResponseCounters_Tests
		// Description: Tests HandleEndRequest response size operations
		[TestMethod]
		public void HandleEndRequest_ResponseCounters_Tests()
		{
            long responseCounter;
			byte[] buffer = Encoding.ASCII.GetBytes("Test");

			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandleBeginRequest(context);

            // ResponseCounter tests
            responseCounter = PerformanceMonitor.ResponseCounter;

			// context.Response.Filter.Length tests
			Assert.IsTrue(context.Response.Filter.Length == 0);

			// mock up a write to the response
			context.Response.Filter.Write(buffer, 0, 4);
			// calling HandleEndRequest
			pm.HandleEndRequest(context);

			// ResponseCounter tests
			Assert.IsTrue(PerformanceMonitor.ResponseCounter == responseCounter + 1);

			// context.Response.Filter.Length tests
			Assert.IsTrue(context.Response.Filter.Length == 115);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeTally == 4);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeMinimum == 4);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeMaximum == 4);


			// calling HandleEndRequest for the second time
			pm.HandleEndRequest(context);

			// context.Response.Filter.Length tests
			Assert.IsTrue(context.Response.Filter.Length == 231);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeTally == 115+4);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeMinimum == 4);
			Assert.IsTrue(PerformanceMonitor.ResponseSizeMaximum == 115);
		}

		// Method:		HandleEndRequest_CriticalException_Tests
		// Description: Tests HandleEndRequest critical failures
		[TestMethod]
		public void HandleEndRequest_CriticalException_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();

			// Critical Exception Handling
			pm.HandleEndRequest(null);
		}

		#endregion

		#region HandlePreRequestHandler

		// Method:		HandlePreRequestHandler_Stopwatch_Tests
		// Description: Tests HandlePreRequestHandler Stopwatch operations
		[TestMethod]
		public void HandlePreRequestHandler_Stopwatch_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandlePreRequestHandler(context);

			// Stopwatch tests
			Pre_Stopwatch_Tests(context, "handler_timer");
		}

		// Method:		HandlePreRequestHandler_CriticalException_Tests
		// Description: Tests HandlePreRequestHandler critical failures
		[TestMethod]
		public void HandlePreRequestHandler_CriticalException_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();

			// Critical Exception Handling
			pm.HandlePreRequestHandler(null);
		}

		#endregion

		#region HandlePostRequestHandler_Tests

		// Method:		HandlePostRequestHandler_Stopwatch_Tests
		// Description: Tests HandlePostRequestHandler Stopwatch operations
		[TestMethod]
		public void HandlePostRequestHandler_Stopwatch_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandlePreRequestHandler(context);
			pm.HandlePostRequestHandler(context);

			// Stopwatch tests
			Post_Stopwatch_Tests(context, "handler_timer");
		}

		// Method:		HandlePostRequestHandler_CriticalException_Tests
		// Description: Tests HandlePostRequestHandler critical failures
		[TestMethod]
		public void HandlePostRequestHandler_CriticalException_Tests()
		{
			// setup for tests
			HttpContextBase context = new MockHttpContext();
			PerformanceMonitor pm = new PerformanceMonitor();

			// Critical Exception Handling
			pm.HandleEndRequest(null);
		}

		#endregion

		// Method:		Pre_Stopwatch_Tests
		// Description: Tests common pre Stopwatch operations
		private void Pre_Stopwatch_Tests(HttpContextBase context, string item)
		{
			Assert.IsTrue(context.Items.Contains(item));
			Assert.IsNotNull(context.Items[item]);
			Assert.IsTrue(context.Items[item].GetType() == typeof(System.Diagnostics.Stopwatch));

            System.Diagnostics.Stopwatch sw = (System.Diagnostics.Stopwatch)context.Items[item];
			Assert.IsNotNull(sw);
			Assert.IsTrue(sw.IsRunning);
			Assert.IsTrue(sw.ElapsedTicks > 0);
			Assert.IsTrue(System.Diagnostics.Stopwatch.Frequency > 0);
			Assert.IsTrue(System.Diagnostics.Stopwatch.IsHighResolution);
		}

		// Method:		Post_Stopwatch_Tests
		// Description: Tests common post Stopwatch operations
		private void Post_Stopwatch_Tests(HttpContextBase context, string item)
		{
			Assert.IsTrue(context.Items.Contains(item));
			Assert.IsNotNull(context.Items[item]);
			Assert.IsTrue(context.Items[item].GetType() == typeof(System.Diagnostics.Stopwatch));

            System.Diagnostics.Stopwatch sw = (System.Diagnostics.Stopwatch)context.Items[item];
			Assert.IsNotNull(sw);
			Assert.IsFalse(sw.IsRunning);
			Assert.IsTrue(sw.ElapsedTicks > 0);
			Assert.IsTrue(System.Diagnostics.Stopwatch.Frequency > 0);
			Assert.IsTrue(System.Diagnostics.Stopwatch.IsHighResolution);
		}
	}
}
