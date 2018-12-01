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

#pragma once
#include "Event.h"
#include "Image.h"
#include "../Math/Color.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(RE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#define RE_NULL_WINDOW 0
#elif defined(RE_PLATFORM_ANDROID)
#define RE_NULL_WINDOW nullptr
#elif defined(RE_PLATFORM_LINUX)
#include <X11/Xlib.h>
#define RE_NULL_WINDOW 0
#else
#define RE_NULL_WINDOW nullptr
#endif


namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(RE_PLATFORM_WINDOWS)
	typedef HWND NativeWindowHandle;
#elif defined(RE_PLATFORM_ANDROID)
	typedef ANativeWindow* NativeWindowHandle;
#elif defined(RE_PLATFORM_LINUX)
	typedef ::Window NativeWindowHandle;
#else
	typedef void* NativeWindowHandle;
#endif

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum WINDOWSTYLE
	{
		WINDOWSTYLE_UNKNOWN = (1 << 0),
		WINDOWSTYLE_BORDERLESS = (1 << 1),
		WINDOWSTYLE_TITLEBAR = (1 << 3),
		WINDOWSTYLE_RESIZEABLE = (1 << 4),
		WINDOWSTYLE_MINIMIZABLE = (1 << 5),
		WINDOWSTYLE_MAXIMIZABLE = (1 << 6),
		WINDOWSTYLE_STANDARD_WINDOW = WINDOWSTYLE_TITLEBAR | WINDOWSTYLE_RESIZEABLE | WINDOWSTYLE_MINIMIZABLE | WINDOWSTYLE_MAXIMIZABLE,
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum WINDOW_FLAG
	{
		WINDOW_FLAG_NONE = 0,
		WINDOW_FLAG_APP_FULLSCREEN = (1 << 0),
		WINDOW_FLAG_APP_NO_SLEEP = (1 << 1),
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct RE_API WindowDesc
	{
		std::string Title;
		int32 Width;
		int32 Height;	
		int32 Style;
		int32 Flags;
		int32 x;
		int32 y;
		Image* pIcon;
		struct
		{
			Math::Point Hotspot;
			Image* pImage;
		} Cursor;

		struct 
		{
			uint8 r;
			uint8 g;
			uint8 b;
		} BackgroundColor;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class RE_API IWindow
	{
		RE_INTERFACE(IWindow);

	public:
		IWindow() {}
		~IWindow() {}

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Displays window
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void Show() const = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Closes the window. No data will be lost, window still needs to be destoyed with a call to 'Destroy()'
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void Close() const = 0;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Gets the next event on the eventqueue if there are any and removes it. Does not block.

			pEvent - A valid pointer to a event

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual bool PeekEvent(Event* pEvent) const = 0;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			Gets the next event on the eventqueue if there are any and removes it. Blocks until it recives an event.

			pEvent - A valid pointer to a event

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void GetEvent(Event* pEvent) const = 0;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the cursor of the window

			pCursor - A valid pointer to an image.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void SetCursor(const Image* pCursor, const Math::Point& hotspot) = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the icon of the window

			pIcon - A valid pointer to an image.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void SetIcon(const Image* pIcon) = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the icon of the window

			pIcon - A valid pointer to an image.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void SetTitle(const std::string& title) = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			Sets background color of the window

			r - Red channel

			g - Green channel

			b - Blue channel

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void SetBackground(uint8 r, uint8 g, uint8 b) = 0;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the width of a window
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual int32 GetWidth() const = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the height of a window
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual int32 GetHeight() const = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			Sets the values in pDesc to the current WindowDesc being used by the window.

			pDesc - A valid pointer to a WindowDesc
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void GetDesc(WindowDesc* pDesc) const = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Destroys the window and frees all memory being used. 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void Destroy() = 0;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the systems native handle for the window. 
			Windows - HWND
			Linux - Window (Xlib)

			pHandle -  A valid pointer to a variable of type NativeWindowHandle
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		virtual void GetNativeWindowHandle(NativeWindowHandle* pHandle) const = 0;
	};
}