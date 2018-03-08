// File:		PerformanceMonitorFactory.h
// Description:	Factory class used for creating PerformanceMonitorHttpModule class.
//
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 

#ifndef __MODULE_FACTORY_H__
#define __MODULE_FACTORY_H__

#include <stdio.h>


// Class:		PerformanceMonitorHttpModuleFactory 
// Description:	Factory class used for creating PerformanceMonitorHttpModule class.
class PerformanceMonitorHttpModuleFactory : public IHttpModuleFactory
{
	public:
		PerformanceMonitorHttpModuleFactory() {}

		virtual HRESULT GetHttpModule( OUT CHttpModule **ppModule, IN IModuleAllocator *)
		{
			HRESULT hr = S_OK;
			PerformanceMonitorHttpModule *pModule = NULL;

			if ( ppModule == NULL )
			{
				hr = HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );
				goto Finished;
			}

			pModule = new PerformanceMonitorHttpModule();
			if ( pModule == NULL )
			{
				hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
				goto Finished;
			}

			*ppModule = pModule;
			pModule = NULL;
	            
		Finished:

			if ( pModule != NULL )
			{
				delete pModule;
				pModule = NULL;
			}

			return hr;
		}

		virtual void
		Terminate()
		{
			delete this;
		}
};

#endif
