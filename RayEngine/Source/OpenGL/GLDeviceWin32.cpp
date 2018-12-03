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

#pragma once
#include "../../Include/OpenGL/GLDeviceWin32.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../Win32/WndclassCache.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::GLDeviceWin32(const DeviceDesc* pDesc)
			: GLDevice(pDesc),
			m_HDC(0),
			m_Hwnd(0),
			m_Context(0),
			m_IsWindowOwner(false)
		{
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::GLDeviceWin32(const DeviceDesc* pDesc, HWND hwnd, int32* pPixelFormatAttribs)
			: GLDevice(pDesc),
			m_HDC(0),
			m_Hwnd(0),
			m_Context(0),
			m_IsWindowOwner(false)
		{
			Create(pDesc, hwnd, pPixelFormatAttribs);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceWin32::~GLDeviceWin32()
		{
			if (m_Context != 0)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(m_Context);
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

			m_Context = wglCreateContext(hDC, 0, attribs);
			wglMakeCurrent(hDC, m_Context);

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

			HDC hDC = GetDC(dummyWindow);
			SetStandardPixelformat(hDC);

			HGLRC hContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hContext);

			if (!QueryWGLExtensions(hDC))
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(hContext);

				ReleaseDC(dummyWindow, hDC);

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
			wglDeleteContext(hContext);

			ReleaseDC(dummyWindow, hDC);

			DestroyWindow(dummyWindow);
			
			if (wglChoosePixelFormat == nullptr || wglCreateContext == nullptr)
			{
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
	}
}

#endif