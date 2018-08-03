#include "..\Include\RefCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		RefCounter::RefCounter()
			: m_ReferenceCounter(0)
		{
		}



		/////////////////////////////////////////////////////////////
		RefCounter::~RefCounter()
		{
		}



		/////////////////////////////////////////////////////////////
		void RefCounter::Release() const
		{
			m_ReferenceCounter--;
			if (m_ReferenceCounter < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* RefCounter::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 RefCounter::GetReferenceCount() const
		{
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		uint32 RefCounter::AddRef()
		{
			m_ReferenceCounter++;
			return m_ReferenceCounter;
		}
	}
}