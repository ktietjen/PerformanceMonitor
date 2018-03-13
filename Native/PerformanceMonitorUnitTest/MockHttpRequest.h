#pragma once
#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <stdio.h>

#include <sal.h>
//  IIS7 Server API header file
#include <httpserv.h>

class MockHttpRequest : IHttpRequest
{
public:
	MockHttpRequest();
	~MockHttpRequest();

	virtual
		HTTP_REQUEST *
		GetRawHttpRequest(
			VOID
		)
	{
		return nullptr;
	}

	virtual
		const HTTP_REQUEST *
		GetRawHttpRequest(
			VOID
		) const
	{
		return nullptr;
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
		return nullptr;
	}

	virtual
		HRESULT
		SetHeader(
			_In_ PCSTR              pszHeaderName,
			_In_reads_bytes_(cchHeaderValue)
			PCSTR                   pszHeaderValue,
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
		PCSTR
		GetHttpMethod(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		HRESULT
		SetHttpMethod(
			_In_ PCSTR              pszHttpMethod
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		SetUrl(
			_In_reads_(cchUrl)
			PCWSTR                  pszUrl,
			_In_ DWORD              cchUrl,
			_In_ BOOL               fResetQueryString
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		SetUrl(
			_In_reads_bytes_(cchUrl)
			PCSTR                   pszUrl,
			_In_ DWORD              cchUrl,
			_In_ BOOL               fResetQueryString
		)
	{
		return S_FALSE;
	}

	virtual
		BOOL
		GetUrlChanged(
			VOID
		) const
	{
		return false;
	}

	virtual
		PCWSTR
		GetForwardedUrl(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		PSOCKADDR
		GetLocalAddress(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		PSOCKADDR
		GetRemoteAddress(
			VOID
		) const
	{
		return nullptr;
	}

	virtual
		HRESULT
		ReadEntityBody(
			_Out_writes_bytes_all_(cbBuffer)
			VOID *                  pvBuffer,
			_In_  DWORD             cbBuffer,
			_In_  BOOL              fAsync,
			_Out_ DWORD *           pcbBytesReceived,
			_Out_ BOOL *            pfCompletionPending = NULL
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		InsertEntityBody(
			_In_reads_bytes_(cbBuffer)
			VOID *                  pvBuffer,
			_In_ DWORD              cbBuffer
		)
	{
		return S_FALSE;
	}

	virtual
		DWORD
		GetRemainingEntityBytes(
			VOID
		)
	{
		return 0;
	}

	virtual
		VOID
		GetHttpVersion(
			_Out_ USHORT *          pMajorVersion,
			_Out_ USHORT *          pMinorVersion
		) const {}

	virtual
		HRESULT
		GetClientCertificate(
			_Outptr_
			HTTP_SSL_CLIENT_CERT_INFO **    ppClientCertInfo,
			_Out_ BOOL *                    pfClientCertNegotiated
		)
	{
		return S_FALSE;
	}

	virtual
		HRESULT
		NegotiateClientCertificate(
			_In_ BOOL               fAsync,
			_Out_ BOOL *            pfCompletionPending = NULL
		)
	{
		return S_FALSE;
	}

	virtual
		DWORD
		GetSiteId(
			VOID
		) const
	{
		return 0;
	}

	virtual
		HRESULT
		GetHeaderChanges(
			_In_ DWORD          dwOldChangeNumber,
			_Out_ DWORD *       pdwNewChangeNumber,
			_Inout_ PCSTR       knownHeaderSnapshot[HttpHeaderRequestMaximum],
			_Inout_ DWORD *     pdwUnknownHeaderSnapshot,
			_Inout_ PCSTR **    ppUnknownHeaderNameSnapshot,
			_Inout_ PCSTR **    ppUnknownHeaderValueSnapshot,
			_Out_writes_all_(HttpHeaderRequestMaximum + 1)
			DWORD               diffedKnownHeaderIndices[HttpHeaderRequestMaximum + 1],
			_Out_ DWORD *       pdwDiffedUnknownHeaders,
			_Out_ DWORD **      ppDiffedUnknownHeaderIndices
		)
	{
		return S_FALSE;
	}
};

