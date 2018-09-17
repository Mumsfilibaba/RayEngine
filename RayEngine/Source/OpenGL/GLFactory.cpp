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
		GLFactory::GLFactory(bool debugLayer)
			: m_References(0)
		{
			AddRef();
			Create(debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLFactory::~GLFactory()
		{
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::EnumerateAdapters(AdapterList& list) const
		{
			list = AdapterList(m_AdapterList.Count);
			for (int32 i = 0; i < m_AdapterList.Count; i++)
				list[i] = m_AdapterList.pAdapters[i];
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::SetName(const std::string& name)
		{
			//Not relevant
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API GLFactory::GetGraphicsApi() const
		{
			return GRAPHICS_API_OPENGL;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLFactory::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLFactory::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLFactory::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
#if defined RE_PLATFORM_WINDOWS
		static LRESULT CALLBACK Win32WinCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (msg == WM_CREATE)
				return static_cast<LRESULT>(0);

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
#endif


		/////////////////////////////////////////////////////////////
		void GLFactory::Create(bool debugLayer)
		{
			int32 deviceID = 0;
			int32 vendorID = 0;


#if defined(RE_PLATFORM_WINDOWS)
			//Create a dummy context
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

			System::NativeWindowHandle dummyWindow = RE_NULL_WINDOW;
			if (System::WndclassCache::Register(wcex))
			{
				dummyWindow = CreateWindow(wcex.lpszClassName, CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, wcex.hInstance, NULL);
			}

			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cRedBits = 0;
			pfd.cRedShift = 0;
			pfd.cGreenBits = 0;
			pfd.cGreenShift = 0;
			pfd.cBlueBits = 0;
			pfd.cBlueShift = 0;
			pfd.cAlphaBits = 0;
			pfd.cAlphaShift = 0;
			pfd.cAccumBits = 0;
			pfd.cAccumRedBits = 0;
			pfd.cAccumGreenBits = 0;
			pfd.cAccumBlueBits = 0;
			pfd.cAlphaBits = 0;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.cAuxBuffers = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;
			pfd.bReserved = 0;
			pfd.dwDamageMask = 0;
			pfd.dwLayerMask = 0;
			pfd.dwVisibleMask = 0;

			GLNativeDevice hDC = GetDC(dummyWindow);
			int32 format = ChoosePixelFormat(hDC, &pfd);
			SetPixelFormat(hDC, format, &pfd);

			HGLRC tempContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, tempContext);


			//Load functions with dummy context
			if (!LoadOpenGL())
				return;


			//Check if we can create a real context
			std::string wglExtensions = wglGetExtensionsStringARB(hDC);
			int32 last = 0;
			for (int32 i = 0; (i = wglExtensions.find(' ', last + 1)) != std::string::npos;)
			{
				m_Extensions.push_back(wglExtensions.substr(last, i - last));
				last = i;
			}


			if (!ExtensionSupported("WGL_ARB_create_context"))
				return;


			wglMakeCurrent(0, 0);
			wglDeleteContext(tempContext);



			//Create real context
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			HGLRC context = wglCreateContextAttribsARB(hDC, 0, attribs);
			wglMakeCurrent(hDC, context);


			int32 version[2];
			glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
			glGetIntegerv(GL_MINOR_VERSION, &version[1]);


			DISPLAY_DEVICE dd = {};
			dd.cb = sizeof(DISPLAY_DEVICE);
			
			std::string id;
			int i = 0;
			while (EnumDisplayDevices(NULL, i, &dd, 0))
			{
				if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				{
					id = dd.DeviceID;
					break;
				}
				i++;
			}

			if (id != "")
			{
				vendorID = std::strtol(id.substr(8, 4).c_str(), nullptr, 16);
				deviceID = std::strtol(id.substr(17, 4).c_str(), nullptr, 16);
			}
#endif

			//--------------------------------------------//
			AdapterInfo adapterInfo = {};
			adapterInfo.VendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
			adapterInfo.ModelName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			adapterInfo.ApiID = 0;
			adapterInfo.DeviceID = deviceID;
			adapterInfo.VendorID = vendorID;
			adapterInfo.Flags = ADAPTER_FLAGS_SWAPCHAIN | ADAPTER_FLAGS_GRAPHICS ;

			if (ExtensionSupported("GL_ARB_compute_shader"))
				adapterInfo.Flags |= ADAPTER_FLAGS_COMPUTE;

			m_AdapterList = AdapterList(1);
			m_AdapterList[0] = adapterInfo;
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::QueryExtensions()
		{
			

			//int32 extensionCount = glGetIntegerv();
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::ExtensionSupported(const std::string& extension) const
		{
			for (int32 i = 0; i < m_Extensions.size(); i++)
			{
				if (m_Extensions[i] == extension)
					return true;
			}

			return false;
		}
	}
}