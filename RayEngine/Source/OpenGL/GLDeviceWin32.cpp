/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in
	compliance with the License.You may obtain a copy of
	the License at

	http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS".MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable.This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "../../Include//Utilities/EngineUtilities.h"
#include "../../Include/OpenGL/GLDeviceWin32.h"
#include "../../Include/OpenGL/GLSwapchainWin32.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../Win32/WndclassCache.h"

#define RE_GL_CLASS_NAME RE_T("GLWND")

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::GLDeviceWin32(const DeviceDesc* pDesc)
			: GLDevice(pDesc),
			m_HDC(0),
			m_Hwnd(0),
			m_IsWindowOwner(false)
		{
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::GLDeviceWin32(const DeviceDesc* pDesc, HWND hwnd, int32* pPixelFormatAttribs)
			: GLDevice(pDesc),
			m_HDC(0),
			m_Hwnd(0),
			m_IsWindowOwner(false)
		{
			Create(pDesc, hwnd, pPixelFormatAttribs);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::~GLDeviceWin32()
		{
			if (m_NativeContext != 0)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(m_NativeContext);
			}

			if (m_HDC != 0)
			{
				ReleaseDC(m_Hwnd, m_HDC);
				m_HDC = 0;
			}

			if (m_IsWindowOwner)
			{
				if (m_Hwnd != 0)
				{
					DestroyWindow(m_Hwnd);
					m_Hwnd = 0;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceWin32::Create(const DeviceDesc* pDesc)
		{
			HWND hWND = CreateDummyWindow();
			if (hWND == 0)
			{
				LOG_ERROR("OpenGL: Failed to create window for context");
				return;
			}

			HDC hDC = GetDC(hWND);
			SetStandardPixelformat(hDC);

			HGLRC hContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hContext);

			if (!QueryWGLExtensions(hDC))
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(hContext);

				ReleaseDC(hWND, hDC);
				
				DestroyWindow(hWND);

				return;
			}

			//Can we create a context?
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContext = nullptr;
			if (!ExtensionSupported("WGL_ARB_create_context"))
			{
				LOG_ERROR("OpenGL: WGL_ARB_create_context is not supported.");
				return;
			}
			else
			{
				wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(LoadFunction("wglCreateContextAttribsARB"));

				wglMakeCurrent(0, 0);
				wglDeleteContext(hContext);
			}

			//Create real context
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
				0
			};

			m_NativeContext = wglCreateContext(hDC, 0, attribs);
			wglMakeCurrent(hDC, m_NativeContext);

			//Did we get a 3.3 or higher
			int32 version[2];
			glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
			glGetIntegerv(GL_MINOR_VERSION, &version[1]);

			if ((version[0] * 10) + version[1] < 33)
			{
				LOG_ERROR("OpenGL: OpenGL version is to low. Only support 3.3 and higher. Current version is: " + std::to_string(version[0]) + '.' + std::to_string(version[1]));

				wglMakeCurrent(0, 0);
			}
			else
			{
				LOG_INFO("OpenGL: Created context. Version " + std::to_string(version[0]) + '.' + std::to_string(version[1]));

				m_HDC = hDC;
				m_Hwnd = hWND;
				m_IsWindowOwner = true;

				LoadOpenGL();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceWin32::Create(const DeviceDesc* pDesc, HWND hwnd, int32* pPixelFormatAttribs)
		{
			HWND dummyWindow = CreateDummyWindow();
			if (dummyWindow == 0)
			{
				LOG_ERROR("OpenGL: Failed to create window for context");
				return;
			}

			HDC dummyDC = GetDC(dummyWindow);
			SetStandardPixelformat(dummyDC);

			HGLRC dummyContext = wglCreateContext(dummyDC);
			wglMakeCurrent(dummyDC, dummyContext);

			if (!QueryWGLExtensions(dummyDC))
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(dummyContext);

				ReleaseDC(dummyWindow, dummyDC);

				DestroyWindow(dummyWindow);

				return;
			}

			//Can we choose pixelformat
			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = nullptr;
			if (!ExtensionSupported("WGL_ARB_pixel_format"))
			{
				LOG_ERROR("OpenGL: WGL_ARB_pixel_format is not supported.");
			}
			else
			{
				wglChoosePixelFormat = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(LoadFunction("wglChoosePixelFormatARB"));
			}

			//Can we create a context?
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContext = nullptr;
			if (!ExtensionSupported("WGL_ARB_create_context"))
			{
				LOG_ERROR("OpenGL: WGL_ARB_create_context is not supported.");
			}
			else
			{
				wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(LoadFunction("wglCreateContextAttribsARB"));
			}

			wglMakeCurrent(0, 0);
			wglDeleteContext(dummyContext);

			ReleaseDC(dummyWindow, dummyDC);

			DestroyWindow(dummyWindow);

			if (wglChoosePixelFormat == nullptr || wglCreateContext == nullptr)
			{
				LOG_ERROR("OpenGL: Failed to get needed funtions.");
				return;
			}

			int32 formatID = 0;
			uint32 formatCount = 0;
			HDC hDC = GetDC(hwnd);
			if (!wglChoosePixelFormat(hDC, pPixelFormatAttribs, nullptr, 1, &formatID, &formatCount))
			{
				ReleaseDC(hwnd, hDC);

				LOG_ERROR("OpengGL: wglChoosePixelFormatARB failed.");
				return;
			}
			else if (formatCount < 1)
			{
				ReleaseDC(hwnd, hDC);

				LOG_ERROR("OpengGL: Did not find any suitable framebuffer pixelformat.");
				return;
			}
			else
			{
				LOG_INFO("OpenGL: Found pixelformat with ID: " + std::to_string(formatID));
				
				PIXELFORMATDESCRIPTOR pfd = {};
				DescribePixelFormat(hDC, formatID, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
				SetPixelFormat(hDC, formatID, &pfd);
			}

			//Create real context
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
				0
			};

			m_NativeContext = wglCreateContext(hDC, 0, attribs);
			wglMakeCurrent(hDC, m_NativeContext);

			//Did we get a 3.3 or higher
			int32 version[2];
			glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
			glGetIntegerv(GL_MINOR_VERSION, &version[1]);

			if ((version[0] * 10) + version[1] < 33)
			{
				LOG_ERROR("OpenGL: OpenGL version is to low. Only support 3.3 and higher. Current version is: " + std::to_string(version[0]) + '.' + std::to_string(version[1]));

				wglMakeCurrent(0, 0);
			}
			else
			{
				LOG_INFO("OpenGL: Created context. Version " + std::to_string(version[0]) + '.' + std::to_string(version[1]));

				m_HDC = hDC;
				m_Hwnd = hwnd;
				m_IsWindowOwner = false;

				LoadOpenGL();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDeviceWin32::QueryWGLExtensions(HDC hDC)
		{
			PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsString = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(LoadFunction("wglGetExtensionsStringARB"));
			if (wglGetExtensionsString == nullptr)
			{
				LOG_ERROR("OpenGL: wglGetExtensionsStringARB is not supported.");
				return false;
			}

			std::string wglExtensions = wglGetExtensionsString(hDC);
			QueryExtensionsFromString(m_Extensions, wglExtensions);

			return true;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LRESULT CALLBACK Win32WinCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (msg == WM_CREATE)
				return static_cast<LRESULT>(0);

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		HWND CreateDummyWindow()
		{
			WNDCLASSEX wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = Win32WinCallback;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = GetModuleHandle(0);
			wcex.hIcon = LoadIcon(wcex.hInstance, static_cast<LPCSTR>(IDI_APPLICATION));
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = RE_GL_CLASS_NAME;
			wcex.hIconSm = LoadIcon(wcex.hInstance, static_cast<LPCSTR>(IDI_APPLICATION));
			
			HWND dummyWindow = 0;
			if (WndclassCache::Register(wcex))
			{
				dummyWindow = CreateWindow(wcex.lpszClassName, RE_GL_CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, wcex.hInstance, NULL);
			}

			return dummyWindow;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetStandardPixelformat(HDC hDC)
		{
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

			int32 format = ChoosePixelFormat(hDC, &pfd);
			SetPixelFormat(hDC, format, &pfd);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void CreateOpenGLDevice(HWND hwnd, IDevice** ppDevice, DeviceDesc* pDeviceDesc, ISwapchain** ppSwapchain, SwapchainDesc* pSwapchainDesc)
		{
			int32 depthBits = FormatDepthBits(pSwapchainDesc->DepthStencil.Format);
			int32 stencilBits = FormatStencilBits(pSwapchainDesc->DepthStencil.Format);
			int32 colorSpace = FormatIsSRGB(pSwapchainDesc->BackBuffer.Format) ? WGL_COLORSPACE_SRGB_EXT : WGL_COLORSPACE_LINEAR_EXT;
			int32 redBits = FormatRedComponentBits(pSwapchainDesc->BackBuffer.Format);
			int32 greenBits = FormatGreenComponentBits(pSwapchainDesc->BackBuffer.Format);
			int32 blueBits = FormatBlueComponentBits(pSwapchainDesc->BackBuffer.Format);
			int32 alphaBits = FormatAlphaComponentBits(pSwapchainDesc->BackBuffer.Format);
			int32 pixelType = FormatIsFloat(pSwapchainDesc->BackBuffer.Format) ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB;
			int32 doubleBuffer = (pSwapchainDesc->BackBuffer.Count > 1) ? GL_TRUE : GL_FALSE;
			if (pSwapchainDesc->BackBuffer.Count > 2)
			{
				LOG_WARNING("OpenGL: Implementation does not support more than 2 buffers in swapchain.");
			}

			int32 pixelFormatAttribs[] =
			{
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, doubleBuffer,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
				WGL_STEREO_ARB, GL_FALSE,
				WGL_COLORSPACE_EXT, colorSpace,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_SAMPLES_ARB, pSwapchainDesc->Samples,
				WGL_PIXEL_TYPE_ARB, pixelType,
				WGL_RED_BITS_ARB, redBits,
				WGL_GREEN_BITS_ARB, greenBits,
				WGL_BLUE_BITS_ARB, blueBits,
				WGL_ALPHA_BITS_ARB, alphaBits,
				depthBits > 0 ? WGL_DEPTH_BITS_ARB : 0 , depthBits,
				stencilBits > 0 ? WGL_STENCIL_BITS_ARB : 0, stencilBits,
				0, 0
			};

			GLDeviceWin32* pDevice = new GLDeviceWin32(pDeviceDesc, hwnd, pixelFormatAttribs);
			*ppDevice = pDevice;

			*ppSwapchain = new GLSwapchainWin32(pSwapchainDesc, pDevice);
		}
	}
}

#endif