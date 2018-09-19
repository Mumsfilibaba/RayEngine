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
#include "..\..\Include\OpenGL\GLFactory.h"
#include "..\..\Include\OpenGL\GLDeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLDevice::GLDevice(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
			: m_References(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<GLFactory*>(pFactory);

			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLDevice::GLDevice(IFactory* pFactory, System::NativeWindowHandle windowHandle, const DeviceInfo& info, bool debugLayer)
		{
			AddRef();
			m_Factory = reinterpret_cast<GLFactory*>(pFactory);

			Create(pFactory, windowHandle, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLDevice::~GLDevice()
		{
			ReRelease_S(m_ImmediateContext);
#if defined(RE_PLATFORM_WINDOWS)
			DeleteDC(m_Device);
#endif
		}



		/////////////////////////////////////////////////////////////
		GLNativeDevice GLDevice::GetGLNativeDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::GetImmediateContext(IDeviceContext** ppContext)
		{
			return (*ppContext = m_ImmediateContext->QueryReference<GLDeviceContext>()) != nullptr;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return new GLDeviceContext(this, true);
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateShader(IShader** ppShader, const ShaderInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateSampler(ISampler** ppSampler, const SamplerInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::QueryFactory(IFactory** ppFactory) const
		{
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		System::Log* GLDevice::GetDeviceLog()
		{
			return &m_Log;
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS
			m_WndHandle = CreateDummyWindow();

			m_Device = GetDC(m_WndHandle);
			SetPixelFormat(m_Device, FORMAT_B8G8R8A8_UNORM, FORMAT_UNKNOWN);

			m_ImmediateContext = new GLDeviceContext(this, false);
#endif
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, System::NativeWindowHandle windowHandle, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS
			GLNativeDevice device = GetDC(windowHandle);

			m_ImmediateContext = new GLDeviceContext(this, false);
#endif
		}
	}
}