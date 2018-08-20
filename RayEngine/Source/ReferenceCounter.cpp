#include "..\Include\ReferenceCounter.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	ReferenceCounter::ReferenceCounter()
		: m_References(0)
	{
	}



	/////////////////////////////////////////////////////////////
	ReferenceCounter::~ReferenceCounter()
	{
	}



	/////////////////////////////////////////////////////////////
	uint32 ReferenceCounter::AddRef()
	{
		m_References++;
		return m_References;
	}



	/////////////////////////////////////////////////////////////
	void ReferenceCounter::Release() const
	{
		m_References--;
		if (m_References < 1)
			delete this;
	}



	/////////////////////////////////////////////////////////////
	uint32 ReferenceCounter::GetReferenceCount() const
	{
		return m_References;
	}



	/////////////////////////////////////////////////////////////
	ReferenceCounter* ReferenceCounter::QueryReference()
	{
		AddRef();
		return this;
	}
}