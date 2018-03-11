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

		// Warning: need to implement everything that WriteBenchmarksToResponse depends upon in MockHttpContext
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

		TEST_METHOD(OnPostPreExecuteRequestHandler)
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

			TEST_METHOD(OnExecuteRequestHandler)
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

			
			TEST_METHOD(OnPostExecuteRequestHandler)
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

			//TODO: Implement MockResponse class && MockHttpContext->AllocateRequestMemory
			TEST_METHOD(OnSendResponse)
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
				//pPerfMonModule->OnSendResponse((IHttpContext*)&context, NULL);

			}
	};
}