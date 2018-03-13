#include "stdafx.h"
#include "MockHttpResponse.h"


MockHttpResponse::MockHttpResponse()
{
	_rawResponse.Version.MajorVersion = 1;
	_rawResponse.Version.MinorVersion = 1;
	_rawResponse.StatusCode = 200;
	_rawResponse.ReasonLength = 0;
	_rawResponse.Headers.UnknownHeaderCount = 0;
	_rawResponse.Headers.pUnknownHeaders = NULL;

	for (int index = 0; index < HttpHeaderResponseMaximum; index++)
	{
		_rawResponse.Headers.KnownHeaders[index].RawValueLength = 0;
		_rawResponse.Headers.KnownHeaders[index].pRawValue = NULL;
	}

	_rawResponse.Headers.KnownHeaders[12].RawValueLength = 10;
	_rawResponse.Headers.KnownHeaders[12].pRawValue = "text/html";

	_rawResponse.EntityChunkCount = 0;
	_rawResponse.pEntityChunks = 0;
}


MockHttpResponse::~MockHttpResponse()
{
}
