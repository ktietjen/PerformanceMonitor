#pragma once
#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <stdio.h>
#include <list>

#include <sal.h>
//  IIS7 Server API header file
#include <httpserv.h>
#include "MockHttpResponse.h"

class MockHttpContext : IHttpContext
{
private:
	// shortcut for allocateding memory for mocking AllocateRequestMemory
	bool _allocatedMemory = true;
	HTTP_DATA_CHUNK _chunk;

	std::list<void *> _allocation_list;

	// mock HttpResponse
	MockHttpResponse _response;

public:
	MockHttpContext();
	~MockHttpContext();

	virtual
		IHttpSite *
		GetSite(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		IHttpApplication *
		GetApplication(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		IHttpConnection *
		GetConnection(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		IHttpRequest *
		GetRequest(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		IHttpResponse *
		GetResponse(
			VOID
		)
	{
		return (IHttpResponse*)&_response;
	}

	virtual
		BOOL
		GetResponseHeadersSent(
			VOID
		) const
	{
		return false;
	}

	virtual
		IHttpUser *
		GetUser(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		IHttpModuleContextContainer *
		GetModuleContextContainer(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		VOID
		IndicateCompletion(
			_In_ REQUEST_NOTIFICATION_STATUS     notificationStatus
		) {}

	virtual
		HRESULT
		PostCompletion(
			_In_ DWORD                cbBytes
		)
	{
		return S_OK;
	}

	virtual
		VOID
		DisableNotifications(
			_In_ DWORD                dwNotifications,
			_In_ DWORD                dwPostNotifications
		) {}

	virtual
		BOOL
		GetNextNotification(
			_In_ REQUEST_NOTIFICATION_STATUS    status,
			_Out_ DWORD *                       pdwNotification,
			_Out_ BOOL *                        pfIsPostNotification,
			_Outptr_ CHttpModule **          ppModuleInfo,
			_Outptr_ IHttpEventProvider **   ppRequestOutput
		)
	{
		return false;
	}

	virtual
		BOOL
		GetIsLastNotification(
			_In_ REQUEST_NOTIFICATION_STATUS   status
		)
	{
		return TRUE;
	}

	virtual
		HRESULT
		ExecuteRequest(
			_In_ BOOL                   fAsync,
			_In_ IHttpContext *         pHttpContext,
			_In_ DWORD                  dwExecuteFlags,
			_In_ IHttpUser *            pHttpUser,
			_Out_ BOOL *                pfCompletionExpected = NULL
		)
	{
		return S_FALSE;
	}

	virtual
		DWORD
		GetExecuteFlags(
			VOID
		) const
	{
		return 0;
	}

	virtual
		HRESULT
		GetServerVariable(
			_In_ PCSTR          pszVariableName,
			_Outptr_result_buffer_(*pcchValueLength)
			PCWSTR *            ppszValue,
			_Out_ DWORD *       pcchValueLength
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		GetServerVariable(
			_In_ PCSTR          pszVariableName,
			_Outptr_result_bytebuffer_(*pcchValueLength)
			PCSTR *             ppszValue,
			_Out_ DWORD *       pcchValueLength
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		SetServerVariable(
			PCSTR               pszVariableName,
			PCWSTR              pszVariableValue
		)
	{
		return S_FALSE;
	}

	virtual

		_Ret_opt_ _Post_writable_byte_size_(cbAllocation)
		VOID *
		AllocateRequestMemory(
			_In_ DWORD          cbAllocation
		)
	{
		void *pAllocation = malloc((size_t)cbAllocation);

		if (pAllocation != NULL)
			_allocation_list.push_back(pAllocation);

		return pAllocation;
	}

	virtual
		IHttpUrlInfo *
		GetUrlInfo(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		IMetadataInfo *
		GetMetadata(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		_Ret_writes_(*pcchPhysicalPath)
		PCWSTR
		GetPhysicalPath(
			_Out_ DWORD *       pcchPhysicalPath = NULL
		)
	{
		return nullptr;
	}

	virtual
		_Ret_writes_(*pcchScriptName)
		PCWSTR
		GetScriptName(
			_Out_ DWORD *       pcchScriptName = NULL
		) const
	{
		return nullptr;
	}

	virtual
		_Ret_writes_(*pcchScriptTranslated)
		PCWSTR
		GetScriptTranslated(
			_Out_ DWORD *       pcchScriptTranslated = NULL
		)
	{
		return nullptr;
	}

	virtual
		IScriptMapInfo *
		GetScriptMap(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		VOID
		SetRequestHandled(
			VOID
		) {}

	virtual
		IHttpFileInfo *
		GetFileInfo(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		HRESULT
		MapPath(
			_In_ PCWSTR         pszUrl,
			_Inout_updates_(*pcbPhysicalPath)
			PWSTR               pszPhysicalPath,
			_Inout_ DWORD *     pcbPhysicalPath
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		NotifyCustomNotification(
			_In_ ICustomNotificationProvider *  pCustomOutput,
			_Out_ BOOL *                        pfCompletionExpected
		)
	{
		return S_FALSE;
	}

	virtual
		IHttpContext *
		GetParentContext(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		IHttpContext *
		GetRootContext(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		HRESULT
		CloneContext(
			_In_ DWORD          dwCloneFlags,
			_Outptr_
			IHttpContext **     ppHttpContext
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		ReleaseClonedContext(
			VOID
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		GetCurrentExecutionStats(
			_Out_ DWORD *   pdwNotification,
			_Out_ DWORD *   pdwNotificationStartTickCount = NULL,
			_Out_ PCWSTR *  ppszModule = NULL,
			_Out_ DWORD *   pdwModuleStartTickCount = NULL,
			_Out_ DWORD *   pdwAsyncNotification = NULL,
			_Out_ DWORD *   pdwAsyncNotificationStartTickCount = NULL
		) const
	{
		return S_FALSE;
	}

	virtual
		IHttpTraceContext *
		GetTraceContext(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		HRESULT
		GetServerVarChanges(
			_In_    DWORD       dwOldChangeNumber,
			_Out_   DWORD *     pdwNewChangeNumber,
			_Inout_ DWORD *     pdwVariableSnapshot,
			_Inout_ _At_(*ppVariableNameSnapshot, _Pre_readable_size_(*pdwVariableSnapshot) _Post_readable_size_(*pdwVariableSnapshot))
			PCSTR **    ppVariableNameSnapshot,
			_Inout_ _At_(*ppVariableValueSnapshot, _Pre_readable_size_(*pdwVariableSnapshot) _Post_readable_size_(*pdwVariableSnapshot))
			PCWSTR **   ppVariableValueSnapshot,
			_Out_   DWORD *     pdwDiffedVariables,
			_Out_   DWORD **    ppDiffedVariableIndices
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		CancelIo(
			VOID
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		MapHandler(
			_In_ DWORD          dwSiteId,
			_In_ PCWSTR         pszSiteName,
			_In_ PCWSTR         pszUrl,
			_In_ PCSTR          pszVerb,
			_Outptr_
			IScriptMapInfo **   ppScriptMap,
			_In_ BOOL           fIgnoreWildcardMappings = FALSE
		)
	{
		return S_FALSE;
	}

	__declspec(deprecated("This method is deprecated. Use the HttpGetExtendedInterface helper function instead."))
		virtual
		HRESULT
		GetExtendedInterface(
			_In_ HTTP_CONTEXT_INTERFACE_VERSION version,
			_Outptr_ PVOID * ppInterface
		)
	{
		return S_FALSE;
	}
};

