#include "..\Include\IReferenceCounter.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	uint32 IReferenceCounter::AddRef()
	{
		ReferenceCounter()++;
		return ReferenceCounter();
	}

	void IReferenceCounter::Release() const
	{
	}

	uint32 IReferenceCounter::GetReferenceCount() const
	{
		return uint32();
	}

	IReferenceCounter* IReferenceCounter::QueryReference()
	{
		AddRef();
		return this;
	}


}