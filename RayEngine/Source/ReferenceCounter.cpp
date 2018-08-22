/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

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