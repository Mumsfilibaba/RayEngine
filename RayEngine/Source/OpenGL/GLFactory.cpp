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
#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLSwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
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

			m_DebugLayer = debugLayer;

			Create(debugLayer);
		}



		/////////////////////////////////////////////////////////////
		GLFactory::~GLFactory()
		{
			ReRelease_S(m_Device);
			ReRelease_S(m_Swapchain);
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
			if (m_Swapchain != nullptr)
			{
				*ppDevice = nullptr;
				return false;
			}
			else if (m_Device != nullptr)
			{
				*ppDevice = m_Device->QueryReference<GLDevice>();
				return true;
			}

			m_Device = new GLDevice(this, deviceInfo, m_DebugLayer);
			*ppDevice = m_Device;
			return true;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo)
		{
			*ppSwapchain = nullptr;
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			if (m_Device != nullptr)
			{
				*ppDevice = nullptr;
				*ppSwapchain = nullptr;
				return false;
			}

			m_Device = new GLDevice(this, swapchainInfo.WindowHandle, deviceInfo, m_DebugLayer);
			*ppDevice = m_Device;

			m_Swapchain = new GLSwapchain(this, m_Device, swapchainInfo);
			*ppSwapchain = m_Swapchain;
			return true;
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


			//Get vendor and device ID
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
			if (LoadOpenGL())
			{
				//At this point we have valid GLfunctions for 3.3
				AdapterInfo adapterInfo = {};
				adapterInfo.VendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
				adapterInfo.ModelName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
				adapterInfo.ApiID = 0;
				adapterInfo.DeviceID = deviceID;
				adapterInfo.VendorID = vendorID;
				adapterInfo.Flags = ADAPTER_FLAGS_SWAPCHAIN | ADAPTER_FLAGS_GRAPHICS;

				if (ExtensionSupported("GL_ARB_compute_shader"))
					adapterInfo.Flags |= ADAPTER_FLAGS_COMPUTE;
				if (ExtensionSupported("GL_ARB_geometry_shader4"))
					adapterInfo.Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
				if (ExtensionSupported("GL_ARB_tessellation_shader"))
					adapterInfo.Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;

				int32 info = 0;
				glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info);
				adapterInfo.Limits.Texture1D.Width = info;

				adapterInfo.Limits.Texture2D.Width = info;
				adapterInfo.Limits.Texture2D.Height = info;

				glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &info);
				adapterInfo.Limits.Texture3D.Width = info;
				adapterInfo.Limits.Texture3D.Height = info;
				adapterInfo.Limits.Texture3D.Depth = info;

				glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &info);
				adapterInfo.Limits.TextureCube.Width = info;
				adapterInfo.Limits.TextureCube.Height = info;

				glGetIntegerv(GL_MAX_DRAW_BUFFERS, &info);
				adapterInfo.Limits.RenderTargetCount = info;


				m_AdapterList = AdapterList(1);
				m_AdapterList[0] = adapterInfo;
			}


#if defined(RE_PLATFORM_WINDOWS)
			wglMakeCurrent(0, 0);
			wglDeleteContext(context);

			DeleteDC(hDC);
			DestroyWindow(dummyWindow);
#endif
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::QueryExtensionsFromString(const std::string & str)
		{
			int32 last = 0;
			for (int32 i = 0; (i = str.find(' ', last)) != std::string::npos;)
			{
				m_Extensions.push_back(str.substr(last, i - last));
				last = i + 1;
			}
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