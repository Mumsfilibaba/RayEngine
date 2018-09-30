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
			mFactory(nullptr),
			mWndHandle(RE_NULL_WINDOW),
			m_NativeDevice(RE_GL_NULL_NATIVE_DEVICE),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);
			mFactory = reinterpret_cast<GLFactory*>(pFactory);

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
			(*ppFactory) = mFactory->QueryReference<GLFactory>();
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::GetReferenceCount() const
		{
			return mReferences;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::Create()
		{
			m_NativeDevice = m_Device->GetGLNativeDevice();
			mWndHandle = m_Device->GetNativeWindowHandle();
		}
	}
}