#include "..\Include\IReferenceCounter.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	uint32 IReferenceCounter::AddRef()
	{
		ReferenceCounter()++;
		return ReferenceCounter();
	}

	IReferenceCounter* IReferenceCounter::QueryReference()
	{
		AddRef();
		return this;
	}


}