#include "stdafx.h"
#include "MockHttpContext.h"


MockHttpContext::MockHttpContext()
{
}


MockHttpContext::~MockHttpContext()
{
	// free AllocatedRequestMemory
	for (std::list<void*>::iterator iter = _allocation_list.begin(); iter != _allocation_list.end(); iter++)
		free(*iter);
}
