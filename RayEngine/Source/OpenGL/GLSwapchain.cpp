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
#include <OpenGL/GLSwapchain.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLSwapchain::GLSwapchain(IDevice* pDevice, const SwapchainDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);
			m_Desc = *pDesc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLSwapchain::~GLSwapchain()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLSwapchain::Resize(int32 width, int32 height)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLSwapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLSwapchain::GetDesc(SwapchainDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLSwapchain::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLSwapchain::Release()
		{
			CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}
	}
}