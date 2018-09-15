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

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
#define CLASS_NAME RE_T("GLWND")
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
			m_Factory = pFactory->QueryReference<GLFactory>();

			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLDevice::GLDevice(IFactory* pFactory, System::NativeWindowHandle windowHandle, const DeviceInfo& info, bool debugLayer)
		{
			AddRef();
			m_Factory = pFactory->QueryReference<GLFactory>();

			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLDevice::~GLDevice()
		{
		}



		/////////////////////////////////////////////////////////////
		GLNativeDevice GLDevice::GetGLNativeDevice() const
		{
			return GLNativeDevice();
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::GetImmediateContext(IDeviceContext** ppContext)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLDevice::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return false;
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
			return nullptr;
		}


#if defined RE_PLATFORM_WINDOWS
		static LRESULT CALLBACK Win32WinCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (msg == WM_CREATE)
				return static_cast<LRESULT>(0);

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
#endif


		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS
			WNDCLASSEX wcex;
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

			System::NativeWindowHandle hWnd = RE_NULL_WINDOW;
			if (System::WndclassCache::Register(wcex))
			{
				hWnd = CreateWindow(wcex.lpszClassName, CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, wcex.hInstance, NULL);
			}

			if (hWnd != RE_NULL_WINDOW)
				Create(pFactory, hWnd, info, debugLayer);
#endif
		}



		/////////////////////////////////////////////////////////////
		void GLDevice::Create(IFactory* pFactory, System::NativeWindowHandle windowHandle, const DeviceInfo& info, bool debugLayer)
		{
#if defined RE_PLATFORM_WINDOWS

#endif
		}
	}
}