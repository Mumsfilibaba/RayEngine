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

			Create(pFactory, info, debugLayer);
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
#if defined(RE_PLATFORM_WINDOWS)
		int32 ChoosePixelFormat(GLNativeDevice hDC, FORMAT backBuffer, FORMAT depthStencil)
		{
			return 0;
		}
#endif


		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS
			//Create a dummy window
			WNDCLASSEX wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = Win32WinCallback;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = GetModuleHandle(0);
			wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = CLASS_NAME;
			wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

			if (System::WndclassCache::Register(wcex))
			{
				m_WndHandle = CreateWindow(wcex.lpszClassName, CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, wcex.hInstance, NULL);
			}

			//Create context
			GLNativeDevice hDC = GetDC(m_WndHandle);
			wglChoosePixelFormatARB();

			int32 format = ChoosePixelFormat(hDC, &pfd);
			SetPixelFormat(hDC, format, &pfd);

			GLNativeContext tempContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, tempContext);


			//Get all extensions for the adapter
			auto wglGetExtensionsString = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(LoadFunction("wglGetExtensionsStringARB"));
			if (wglGetExtensionsString == nullptr)
				return;

			std::string wglExtensions = wglGetExtensionsString(hDC);
			QueryExtensionsFromString(wglExtensions);

			std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
			QueryExtensionsFromString(extensions);


			//Can we create a context?
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContext = nullptr;
			if (!ExtensionSupported("WGL_ARB_create_context"))
			{
				return;
			}
			else
			{
				wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(LoadFunction("wglCreateContextAttribsARB"));

				wglMakeCurrent(0, 0);
				wglDeleteContext(tempContext);
			}


			//Create real context
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			GLNativeContext context = wglCreateContext(hDC, 0, attribs);
			wglMakeCurrent(hDC, context);

			//Did we get a 3.3 or higher
			int32 version[2];
			glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
			glGetIntegerv(GL_MINOR_VERSION, &version[1]);

			if ((version[0] * 10) + version[1] < 33)
				return;
#endif
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, System::NativeWindowHandle windowHandle, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS
			GLNativeDevice device = GetDC(windowHandle);
#endif
		}
	}
}