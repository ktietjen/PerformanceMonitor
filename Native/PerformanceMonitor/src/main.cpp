// File:		main.cpp
// Description:	RegisterModule DLL entry point for PerformanceMonitor IIS Http Module.
//  
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 

#include "precomp.h"


IHttpServer *g_pHttpServer = NULL;  //  Global server instance
PVOID g_pModuleContext = NULL;  //  Global module context id

// Function:	RegisterModule
// Description: IIS calls RegisterModule to load Http Modules DLL and create the module factory and register for events
HRESULT __stdcall RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo *pModuleInfo, IHttpServer *pHttpServer)
{
    HRESULT hr = S_OK;
    PerformanceMonitorHttpModuleFactory *pFactory = NULL;

    if ( pModuleInfo == NULL || pHttpServer == NULL )
    {
        hr = HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );
    }
	else
	{
		// Cache the IHttpServer and the module context id in case we need it later
		g_pHttpServer = pHttpServer;
		g_pModuleContext = pModuleInfo->GetId();
		
		// Create the module factory
		pFactory = new PerformanceMonitorHttpModuleFactory();
		if ( pFactory == NULL )
		{
			hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
		}
		else
		{
			// Register for server events
			DWORD dwRequestNotifications = RQ_BEGIN_REQUEST | RQ_END_REQUEST | RQ_EXECUTE_REQUEST_HANDLER | RQ_SEND_RESPONSE;
			DWORD dwPostRequestNotifications = RQ_PRE_EXECUTE_REQUEST_HANDLER | RQ_EXECUTE_REQUEST_HANDLER;

			hr = pModuleInfo->SetRequestNotifications( pFactory, dwRequestNotifications, dwPostRequestNotifications);
		}
	} 

    return hr;
}


BOOL APIENTRY
DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}
	return TRUE;
}