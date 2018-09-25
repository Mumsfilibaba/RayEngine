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

#include "..\..\Include\OpenGL\GLSwapchain.h"
#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLFactory.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLSwapchain::GLSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info)
			: m_Device(nullptr),
			m_Factory(nullptr),
			m_WndHandle(RE_NULL_WINDOW),
			m_NativeDevice(RE_GL_NULL_NATIVE_DEVICE),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);
			m_Factory = reinterpret_cast<GLFactory*>(pFactory);

			Create();
		}



		/////////////////////////////////////////////////////////////
		GLSwapchain::~GLSwapchain()
		{
		}


		/////////////////////////////////////////////////////////////
		void GLSwapchain::Resize(int32 width, int32 height)
		{
			//Not relevant
		}


		/////////////////////////////////////////////////////////////
		void GLSwapchain::Present() const
		{
			SwapBuffers(m_NativeDevice);
		}


		/////////////////////////////////////////////////////////////
		void GLSwapchain::SetName(const std::string& name)
		{
			//Not relevant
		}


		/////////////////////////////////////////////////////////////
		void GLSwapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		/////////////////////////////////////////////////////////////
		void GLSwapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = m_Factory->QueryReference<GLFactory>();
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::GetReferenceCount() const
		{
			return m_References;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::Create()
		{
			m_NativeDevice = m_Device->GetGLNativeDevice();
			m_WndHandle = m_Device->GetNativeWindowHandle();
		}
	}
}