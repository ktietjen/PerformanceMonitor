// File:		PerformanceMonitorFactoryUnitTests.h
// Description:	All unit tests for tactory class used for creating PerformanceMonitorHttpModule class.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\PerformanceMonitor\inc\precomp.h"

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

	};
}