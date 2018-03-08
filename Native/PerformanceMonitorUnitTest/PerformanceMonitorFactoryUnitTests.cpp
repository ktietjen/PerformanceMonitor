// File:		PerformanceMonitorFactoryUnitTests.h
// Description:	All unit tests for tactory class used for creating PerformanceMonitorHttpModule class.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\PerformanceMonitor\inc\precomp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PerformanceMonitorUnitTest
{
	// Class:		PerformanceMonitorFactoryUnitTests 
	// Description:	Class that defines unit tests for PerformanceMonitorFactory that tests
	//				to make sure that the factory will generate a PerformanceMonitor object.
	TEST_CLASS(PerformanceMonitorFactoryUnitTests)
	{
	public:


		TEST_METHOD(FactoryTestMethod)
		{
			HRESULT hr;
			CHttpModule *pModule;
			PerformanceMonitorHttpModuleFactory factory;

			hr = factory.GetHttpModule(&pModule, NULL);

			Assert::IsTrue(hr == S_OK);
			Assert::IsNotNull(pModule);
		}

	};
}