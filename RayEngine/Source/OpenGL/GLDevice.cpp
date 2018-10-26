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

#include "..\..\Include\System\Log\LogService.h"
#include "..\..\Include\OpenGL\GLFactory.h"
#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLDeviceContext.h"
#include "..\..\Include\OpenGL\GLShader.h"
#include "..\..\Include\OpenGL\GLRootLayout.h"
#include "..\..\Include\OpenGL\GLPipelineState.h"
#include "..\..\Include\OpenGL\GLBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
#endif

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDevice::GLDevice(IFactory* pFactory, const DeviceDesc* pDesc, bool debugLayer)
			: m_ImmediateContext(nullptr),
			m_Device(RE_GL_NULL_NATIVE_DEVICE),
			m_WndHandle(RE_NULL_WINDOW),
			m_CreatedWindow(false),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<GLFactory*>(pFactory);

			m_Desc = *pDesc;

			Create(debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDevice::GLDevice(IFactory* pFactory, System::NativeWindowHandle nativeWindow, GLNativeDevice nativeDevice, const DeviceDesc* pDesc, bool debugLayer)
			: m_ImmediateContext(nullptr),
			m_Device(RE_GL_NULL_NATIVE_DEVICE),
			m_WndHandle(RE_NULL_WINDOW),
			m_CreatedWindow(false),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<GLFactory*>(pFactory);

			m_Desc = *pDesc;

			Create(nativeWindow, nativeDevice, debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDevice::~GLDevice()
		{
			ReRelease_S(m_ImmediateContext);

#if defined(RE_PLATFORM_WINDOWS)
			if (m_Device != RE_GL_NULL_NATIVE_DEVICE)
				ReleaseDC(m_WndHandle, m_Device);

			if (m_WndHandle != RE_NULL_WINDOW && m_CreatedWindow)
				DestroyWindow(m_WndHandle);
#endif
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::GetImmediateContext(IDeviceContext** ppContext)
		{
			return (*ppContext = m_ImmediateContext->QueryReference<GLDeviceContext>()) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return new GLDeviceContext(this, true);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateShader(IShader** ppShader, const ShaderDesc* pDesc)
		{
			return (*ppShader = new GLShader(this, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			return (*ppBuffer) = new GLBuffer(this, pInitialData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc)
		{
			return (*ppRootLayout = new GLRootLayout(this, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc)
		{
			return (*ppPipelineState = new GLPipelineState(this, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = m_Factory->QueryReference<GLFactory>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::Release()
		{
			IObject::CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::Create(bool debugLayer)
		{
#if defined(RE_PLATFORM_WINDOWS)
			m_WndHandle = CreateDummyWindow();
			m_CreatedWindow = true;
			
			m_Device = GetDC(m_WndHandle);
			if (!SetPixelFormat(m_Device, FORMAT_B8G8R8A8_UNORM, FORMAT_UNKNOWN))
			{
				return;
			}
#endif

			CreateContext(debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::Create(System::NativeWindowHandle nativeWindow, GLNativeDevice nativeDevice, bool debugLayer)
		{
			m_WndHandle = nativeWindow;
			m_Device = nativeDevice;

			CreateContext(debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::CreateContext(bool debugLayer)
		{
			CreateNativeContext(debugLayer);
			m_ImmediateContext = new GLDeviceContext(this, false);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::CreateNativeContext(bool debugLayer)
		{
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, 0,//debugLayer ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
				0
			};

			SetLastError(0);
			m_NativeContext = wglCreateContextAttribsARB(m_Device, 0, attribs);
			if (m_NativeContext == 0)
			{
				DWORD error = GetLastError();
				wglMakeCurrent(0, 0);
				return;
			}

			wglMakeCurrent(m_Device, m_NativeContext);


			//Did we get a 3.3 or higher
			int32 version[2];
			glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
			glGetIntegerv(GL_MINOR_VERSION, &version[1]);


			//Version to low - delete context
			if ((version[0] * 10) + version[1] < 33)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(m_NativeContext);
			}


			//Get extensions for device (Should be the same as factory)
			std::string wglExtensions = wglGetExtensionsStringARB(m_Device);

			int32 numExtensions = 0;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
			for (int32 i = 0; i < numExtensions; i++)
			{
				const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
				mExtensions.push_back(extension);
			}
		}
	}
}