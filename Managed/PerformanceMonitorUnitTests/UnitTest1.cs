using System;
using System.Web;
using HttpModule;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace PerformanceMonitorUnitTests
{
	public class MockHttpContext : HttpContextBase
	{
		Dictionary<string, Object> items = new Dictionary<string, object>();
		MockResponseContext response = new MockResponseContext();

		public MockHttpContext()
		{
			response.Filter = new MemoryStream();
		}
		public override IDictionary Items { get { return items; } }
		public override HttpResponseBase Response { get { return response; } }
	}

	public class MockResponseContext : HttpResponseBase
	{
		public override Stream Filter { get; set; }
	}


	[TestClass]
	public class UnitTest1
	{
		[TestMethod]
		public void TestMethod1()
		{

			HttpContextBase context = new MockHttpContext();

			PerformanceMonitor pm = new PerformanceMonitor();
			pm.HandleBeginRequest(context);

			Assert.IsTrue(context.Items.Contains("request_timer"));
			Assert.IsTrue(context.Items["request_timer"].GetType() == typeof(Stopwatch));
		}
	}
}
