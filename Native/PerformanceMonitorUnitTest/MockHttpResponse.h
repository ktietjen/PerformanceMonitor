#pragma once
#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <stdio.h>

#include <sal.h>
//  IIS7 Server API header file
#include <httpserv.h>

class MockHttpResponse
{
private:
	HTTP_RESPONSE _rawResponse;

public:
	MockHttpResponse();
	~MockHttpResponse();

	virtual
		HTTP_RESPONSE *
		GetRawHttpResponse(
			VOID
		)
	{
		return &_rawResponse;
	}

	virtual
		const HTTP_RESPONSE *
		GetRawHttpResponse(
			VOID
		) const
	{
		return &_rawResponse;
	}

	virtual
		IHttpCachePolicy *
		GetCachePolicy(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		HRESULT
		SetStatus(
			_In_ USHORT                         statusCode,
			_In_ PCSTR                          pszReason,
			_In_ USHORT                         uSubStatus = 0,
			_In_ HRESULT                        hrErrorToReport = S_OK,
			_In_opt_ IAppHostConfigException *  pException = NULL,
			_In_ BOOL                           fTrySkipCustomErrors = FALSE
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		SetHeader(
			_In_ PCSTR              pszHeaderName,
			_In_ PCSTR              pszHeaderValue,
			_In_ USHORT             cchHeaderValue,
			_In_ BOOL               fReplace
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		SetHeader(
			_In_ HTTP_HEADER_ID     ulHeaderIndex,
			_In_ PCSTR              pszHeaderValue,
			_In_ USHORT             cchHeaderValue,
			_In_ BOOL               fReplace
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		DeleteHeader(
			_In_ PCSTR              pszHeaderName
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		DeleteHeader(
			_In_ HTTP_HEADER_ID     ulHeaderIndex
		)
	{
		return S_FALSE;
	}

	virtual
		_Ret_writes_bytes_(*pcchHeaderValue)
		PCSTR
		GetHeader(
			_In_ PCSTR              pszHeaderName,
			_Out_ USHORT *          pcchHeaderValue = NULL
		) const
	{
		return nullptr;
	}

	virtual
		_Ret_writes_bytes_(*pcchHeaderValue)
		PCSTR
		GetHeader(
			_In_  HTTP_HEADER_ID    ulHeaderIndex,
			_Out_ USHORT *          pcchHeaderValue = NULL
		) const
	{
		*pcchHeaderValue = _rawResponse.Headers.KnownHeaders[ulHeaderIndex].RawValueLength;
		return _rawResponse.Headers.KnownHeaders[ulHeaderIndex].pRawValue;
	}

	virtual
		VOID
		Clear(
			VOID
		) {}

	virtual
		VOID
		ClearHeaders(
			VOID
		) {}

	virtual
		VOID
		SetNeedDisconnect(
			VOID
		) {}

	virtual
		VOID
		ResetConnection(
			VOID
		) {}

	virtual
		VOID
		DisableKernelCache(
			ULONG reason = 9
		) {}

	virtual
		BOOL
		GetKernelCacheEnabled(
			VOID
		) const
	{
		return false;
	}

	virtual
		VOID
		SuppressHeaders(
			VOID
		) {}

	virtual
		BOOL
		GetHeadersSuppressed(
			VOID
		) const
	{
		return false;
	}

	virtual
		HRESULT
		Flush(
			_In_ BOOL               fAsync,
			_In_ BOOL               fMoreData,
			_Out_ DWORD *           pcbSent,
			_Out_ BOOL *            pfCompletionExpected = NULL
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		Redirect(
			_In_ PCSTR              pszUrl,
			_In_ BOOL               fResetStatusCode = TRUE,
			_In_ BOOL               fIncludeParameters = FALSE
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		WriteEntityChunkByReference(
			_In_ HTTP_DATA_CHUNK *  pDataChunk,
			_In_ LONG               lInsertPosition = -1
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		WriteEntityChunks(
			_In_reads_(nChunks)
			HTTP_DATA_CHUNK *       pDataChunks,
			_In_  DWORD             nChunks,
			_In_  BOOL              fAsync,
			_In_  BOOL              fMoreData,
			_Out_ DWORD *           pcbSent,
			_Out_ BOOL *            pfCompletionExpected = NULL
		)
	{
		return S_FALSE;
	}

	virtual
		VOID
		DisableBuffering(
			VOID
		) {}

	virtual
		VOID
		GetStatus(
			_Out_ USHORT *              pStatusCode,
			_Out_ USHORT *              pSubStatus = NULL,
			_Outptr_opt_result_bytebuffer_(*pcchReason)
			PCSTR *                     ppszReason = NULL,
			_Out_ USHORT *              pcchReason = NULL,
			_Out_ HRESULT *             phrErrorToReport = NULL,
			_Outptr_opt_
			PCWSTR *                    ppszModule = NULL,
			_Out_ DWORD *               pdwNotification = NULL,
			_Outptr_opt_
			IAppHostConfigException **  ppException = NULL,
			_Out_ BOOL *                pfTrySkipCustomErrors = NULL
		) {}

	virtual
		HRESULT
		SetErrorDescription(
			_In_reads_(cchDescription)
			PCWSTR                      pszDescription,
			_In_ DWORD                  cchDescription,
			_In_ BOOL                   fHtmlEncode = TRUE
		)
	{
		return S_FALSE;
	}

	virtual
		_Ret_writes_(*pcchDescription)
		PCWSTR
		GetErrorDescription(
			_Out_ DWORD *               pcchDescription = NULL
		)
	{
		return nullptr;
	}

	virtual
		HRESULT
		GetHeaderChanges(
			_In_ DWORD          dwOldChangeNumber,
			_Out_ DWORD *       pdwNewChangeNumber,
			_Inout_ PCSTR       knownHeaderSnapshot[HttpHeaderResponseMaximum],
			_Inout_ DWORD *     pdwUnknownHeaderSnapshot,
			_Inout_ PCSTR **    ppUnknownHeaderNameSnapshot,
			_Inout_ PCSTR **    ppUnknownHeaderValueSnapshot,
			_Out_writes_(HttpHeaderResponseMaximum + 1)
			DWORD               diffedKnownHeaderIndices[HttpHeaderResponseMaximum + 1],
			_Out_ DWORD *       pdwDiffedUnknownHeaders,
			_Out_ DWORD **      ppDiffedUnknownHeaderIndices
		)
	{
		return S_FALSE;
	}

	virtual
		VOID
		CloseConnection(
			VOID
		) {}
};

