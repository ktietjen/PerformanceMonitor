// File:		PerformanceMonitorFactoryUnitTests.h
// Description:	All unit tests for tactory class used for creating PerformanceMonitorHttpModule class.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\PerformanceMonitor\inc\precomp.h"
#include "MockHttpContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PerformanceMonitorUnitTest

{		
	// Class:		PerformanceMonitorUnitTests 
	// Description:	Class that defines unit tests for PerformanceMonitor that tests each of the event
	//				handlers for functionality and critical failures.
	TEST_CLASS(PerformanceMonitorUnitTests)
	{
	public:

		// Method:		OnBeginRequestTestMethod
		// Description: Tests OnBeginRequest
		TEST_METHOD(OnBeginRequestTestMethod)
		{
			HRESULT hr;
			CHttpModule *pModule;
			PerformanceMonitorHttpModule *pPerfMonModule;
			PerformanceMonitorHttpModuleFactory factory;

			hr = factory.GetHttpModule(&pModule, NULL);

			Assert::IsTrue(hr == S_OK);
			Assert::IsNotNull(pModule);

			pModule->OnBeginRequest(NULL, NULL);

			pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

			Assert::IsNotNull(pPerfMonModule);
			Assert::IsTrue(pPerfMonModule->_requestTimer.IsRunning());
		}

		// Method:		OnBeginRequestTestMethod
		// Description: Tests OnBeginRequest
		TEST_METHOD(OnEndRequestTestMethod)
		{
			HRESULT hr;
			CHttpModule *pModule;
			PerformanceMonitorHttpModule *pPerfMonModule;
			PerformanceMonitorHttpModuleFactory factory;
			MockHttpContext context;

			hr = factory.GetHttpModule(&pModule, NULL);

			Assert::IsTrue(hr == S_OK);
			Assert::IsNotNull(pModule);

			pModule->OnBeginRequest(NULL, NULL);

			pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

			Assert::IsNotNull(pPerfMonModule);
			Assert::IsTrue(pPerfMonModule->_requestTimer.IsRunning());

			Assert::IsTrue(pPerfMonModule->g_responseCount == 0);

			pPerfMonModule->OnEndRequest((IHttpContext*)&context, NULL);

			Assert::IsTrue(pPerfMonModule->_endRequestCalled);
			Assert::IsFalse(pPerfMonModule->_requestTimer.IsRunning());
			Assert::IsTrue(pPerfMonModule->g_responseCount == 1);
		}

		// Method:		OnPostPreExecuteRequestHandlerTestMethod
		// Description: Tests OnPostPreExecuteRequestHandler
		TEST_METHOD(OnPostPreExecuteRequestHandlerTestMethod)
		{
			HRESULT hr;
			CHttpModule *pModule;
			PerformanceMonitorHttpModule *pPerfMonModule;
			PerformanceMonitorHttpModuleFactory factory;

			hr = factory.GetHttpModule(&pModule, NULL);

			Assert::IsTrue(hr == S_OK);
			Assert::IsNotNull(pModule);

			pModule->OnPostPreExecuteRequestHandler(NULL, NULL);

			pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

			Assert::IsNotNull(pPerfMonModule);
			Assert::IsTrue(pPerfMonModule->_handlerTimer.IsRunning());
		}

		// Method:		OnExecuteRequestHandlerTestMethod
		// Description: Tests OnExecuteRequestHandler
			TEST_METHOD(OnExecuteRequestHandlerTestMethod)
		{
			HRESULT hr;
			CHttpModule *pModule;
			PerformanceMonitorHttpModule *pPerfMonModule;
			PerformanceMonitorHttpModuleFactory factory;

			hr = factory.GetHttpModule(&pModule, NULL);

			Assert::IsTrue(hr == S_OK);
			Assert::IsNotNull(pModule);

			pModule->OnExecuteRequestHandler(NULL, NULL);

			pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

			Assert::IsNotNull(pPerfMonModule);
			Assert::IsTrue(pPerfMonModule->_handlerTimer.IsRunning());
		}

			// Method:		OnPostExecuteRequestHandlerTestMethod
			// Description: Tests OnPostExecuteRequestHandler
			TEST_METHOD(OnPostExecuteRequestHandlerTestMethod)
			{
				HRESULT hr;
				CHttpModule *pModule;
				PerformanceMonitorHttpModule *pPerfMonModule;
				PerformanceMonitorHttpModuleFactory factory;

				hr = factory.GetHttpModule(&pModule, NULL);

				Assert::IsTrue(hr == S_OK);
				Assert::IsNotNull(pModule);

				pModule->OnPostPreExecuteRequestHandler(NULL, NULL);

				pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

				Assert::IsNotNull(pPerfMonModule);
				Assert::IsTrue(pPerfMonModule->_handlerTimer.IsRunning());

				pPerfMonModule->OnPostExecuteRequestHandler(NULL, NULL);

				Assert::IsFalse(pPerfMonModule->_handlerTimer.IsRunning());
			}

			// Method:		OnSendResponseTestMethod
			// Description: Tests OnSendResponse, UpdateRequestSizeBenchmarks and WriteBenchmarksToResponse methods
			TEST_METHOD(OnSendResponseTestMethod)
			{
				HRESULT hr;
				PerformanceMonitorHttpModuleFactory factory;
				CHttpModule *pModule;
				PerformanceMonitorHttpModule *pPerfMonModule;
				MockHttpContext context;

				hr = factory.GetHttpModule(&pModule, NULL);

				pPerfMonModule = dynamic_cast<PerformanceMonitorHttpModule*>(pModule);

				pPerfMonModule->OnBeginRequest(NULL, NULL);
				pPerfMonModule->OnExecuteRequestHandler(NULL, NULL);
				pPerfMonModule->OnPostExecuteRequestHandler(NULL, NULL);
				pPerfMonModule->OnEndRequest((IHttpContext*)&context, NULL);
				pPerfMonModule->OnSendResponse((IHttpContext*)&context, NULL);

				Assert::IsTrue(pPerfMonModule->_sendResponseCalled);
				Assert::IsTrue(pPerfMonModule->_responseSize == 41);
				Assert::IsTrue(pPerfMonModule->g_minimumResponseSize == 41);
				Assert::IsTrue(pPerfMonModule->g_maximumResponseSize == 41);
				Assert::IsTrue(pPerfMonModule->g_totalResponseSize == 41);
			}
	};
}