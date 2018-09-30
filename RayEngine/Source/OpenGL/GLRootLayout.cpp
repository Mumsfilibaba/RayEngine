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

#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLRootLayout.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLRootLayout::GLRootLayout(IDevice* pDevice, const RootLayoutInfo& info)
			: m_Device(nullptr),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(info);
		}


		/////////////////////////////////////////////////////////////
		GLRootLayout::~GLRootLayout()
		{
		}


		/////////////////////////////////////////////////////////////
		void GLRootLayout::Create(const RootLayoutInfo& info)
		{
		}


		/////////////////////////////////////////////////////////////
		void GLRootLayout::SetName(const std::string& name)
		{
		}


		/////////////////////////////////////////////////////////////
		void GLRootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLRootLayout::GetReferenceCount() const
		{
			return mReferences;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLRootLayout::Release()
		{
			IObject::CounterType refs = --mReferences;
			if (refs < 1)
				delete this;

			return refs;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLRootLayout::AddRef()
		{
			return ++mReferences;
		}
	}
}