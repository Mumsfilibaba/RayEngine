#include "..\..\Include\OpenGL\GLCommon.h"
#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
#endif

namespace RayEngine
{
	namespace Graphics
	{
#if defined(RE_PLATFORM_WINDOWS)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 GetColorSpace(FORMAT format)
		{
			switch (format)
			{
			case FORMAT_R8G8B8A8_UNORM:
			case FORMAT_R8G8B8A8_UINT:
			case FORMAT_R8G8B8A8_SNORM:
			case FORMAT_R8G8B8A8_SINT:
			case FORMAT_B8G8R8A8_UNORM:			return WGL_COLORSPACE_LINEAR_EXT;
			case FORMAT_R8G8B8A8_UNORM_SRGB:
			case FORMAT_B8G8R8A8_UNORM_SRGB:	return WGL_COLORSPACE_SRGB_EXT;
			}

			return 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 GetColorBits(FORMAT format)
		{
			switch (format)
			{
			case FORMAT_R8G8B8A8_UNORM:
			case FORMAT_R8G8B8A8_UINT:
			case FORMAT_R8G8B8A8_SNORM:
			case FORMAT_R8G8B8A8_SINT:
			case FORMAT_B8G8R8A8_UNORM:
			case FORMAT_R8G8B8A8_UNORM_SRGB:
			case FORMAT_B8G8R8A8_UNORM_SRGB:	return 32;
			}

			return 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 GetDepthBits(FORMAT format)
		{
			switch (format)
			{
			case FORMAT_D16_UNORM: return 16;
			case FORMAT_D24_UNORM_S8_UINT: return 24;
			case FORMAT_D32_FLOAT: return 32;
			}

			return 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 GetStencilBits(FORMAT format)
		{
			return format == FORMAT_D24_UNORM_S8_UINT ? 8 : 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool SetPixelFormat(GLNativeDevice hDC, FORMAT backBuffer, FORMAT depthStencil)
		{
			int32 depthBits = GetDepthBits(depthStencil);
			int32 stencilBits = GetStencilBits(depthStencil);

			//Setup general attribs
			int32 attrib[] =
			{
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
				WGL_STEREO_ARB, GL_FALSE,
				WGL_COLORSPACE_EXT, GetColorSpace(backBuffer),
				WGL_SAMPLE_BUFFERS_ARB, 1,
				WGL_SAMPLES_ARB, 1,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, GetColorBits(backBuffer),
				depthBits > 0 ? WGL_DEPTH_BITS_ARB : 0 , depthBits,
				stencilBits > 0 ? WGL_STENCIL_BITS_ARB : 0, stencilBits,
				0, 0
			};

			int32 pixelFormat = 0;
			uint32 numFormats = 0;
			DWORD result = wglChoosePixelFormatARB(hDC, attrib, 0, 1, &pixelFormat, &numFormats);
			if (result)
			{
				PIXELFORMATDESCRIPTOR pfd = {};
				DescribePixelFormat(hDC, pixelFormat, sizeof(pfd), &pfd);

				return SetPixelFormat(hDC, pixelFormat, &pfd);
			}
			else
			{
				if (GetColorSpace(backBuffer) != WGL_COLORSPACE_LINEAR_EXT)
					return false;

				PIXELFORMATDESCRIPTOR pfd = {};
				pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
				pfd.nVersion = 1;
				pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				pfd.iPixelType = PFD_TYPE_RGBA;
				pfd.cColorBits = GetColorBits(backBuffer);
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
				pfd.cDepthBits = GetDepthBits(depthStencil);
				pfd.cStencilBits = GetStencilBits(depthStencil);
				pfd.cAuxBuffers = 0;
				pfd.iLayerType = PFD_MAIN_PLANE;
				pfd.bReserved = 0;
				pfd.dwDamageMask = 0;
				pfd.dwLayerMask = 0;
				pfd.dwVisibleMask = 0;

				int32 format = ChoosePixelFormat(hDC, &pfd);
				if (format)
				{
					return SetPixelFormat(hDC, format, &pfd);
				}
			}

			return false;
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
			if (System::WndclassCache::Register(wcex))
			{
				dummyWindow = CreateWindow(wcex.lpszClassName, RE_GL_CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, wcex.hInstance, NULL);
			}

			return dummyWindow;
		}
#endif


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void QueryExtensionsFromString(std::vector<std::string>& extensions, const std::string& str)
		{
			int32 last = 0;
			for (int32 i = 0; (i = static_cast<int32>(str.find(' ', last))) != std::string::npos;)
			{
				extensions.push_back(str.substr(last, i - last));
				last = i + 1;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLNativeContext GetCurrentContext()
		{
#if defined(RE_PLATFORM_WINDOWS)
			return wglGetCurrentContext();
#endif
		}
	}
}