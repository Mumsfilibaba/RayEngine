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

#include <RayEngine.h>
#include <OpenGL/GLDevice.h>
#include <OpenGL/GLRootLayout.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLRootLayout::GLRootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLRootLayout::~GLRootLayout()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLRootLayout::Create(const RootLayoutDesc* pDesc)
		{
			m_Desc = *pDesc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLRootLayout::GetDesc(RootLayoutDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLRootLayout::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLRootLayout::Release()
		{
			CounterType refs = --m_References;
			if (refs < 1)
				delete this;

			return refs;
		}
	}
}