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
#include "Bitmap.h"
#include "..\Math\Color.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(RE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#define RE_NULL_WINDOW 0
#elif defined(RE_PLATFORM_ANDROID)
#define RE_NULL_WINDOW nullptr
#else
#define RE_NULL_WINDOW nullptr
#endif



namespace RayEngine
{
	namespace System
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(RE_PLATFORM_WINDOWS)
		typedef HWND NativeWindowHandle;
#elif defined(RE_PLATFORM_ANDROID)
		typedef ANativeWindow* NativeWindowHandle;
#else
		typedef void* NativeWindowHandle;
#endif


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum WINDOWSTYLE
		{
			WINDOWSTYLE_UNKNOWN = (1 << 0),
			WINDOWSTYLE_BORDERLESS = (1 << 1),
			WINDOWSTYLE_WINDOW = (1 << 2),
			WINDOWSTYLE_TITLEBAR = (1 << 3),
			WINDOWSTYLE_SYSTEMMENU = (1 << 4),
			WINDOWSTYLE_RESIZEABLE = (1 << 5),
			WINDOWSTYLE_MINIMIZABLE = (1 << 6),
			WINDOWSTYLE_MAXIMIZABLE = (1 << 7),
			WINDOWSTYLE_STANDARD_WINDOW = WINDOWSTYLE_WINDOW | WINDOWSTYLE_TITLEBAR | WINDOWSTYLE_SYSTEMMENU | 
				WINDOWSTYLE_RESIZEABLE | WINDOWSTYLE_MINIMIZABLE | WINDOWSTYLE_MAXIMIZABLE,
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum WINDOW_FLAG
		{
			WINDOW_FLAG_NONE = 0,
			WINDOW_FLAG_APP_FULLSCREEN = (1 << 0),
			WINDOW_FLAG_APP_NO_SLEEP = (1 << 1),
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Struct to decribe a window
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RE_API WindowDesc
		{
			//TODO: Title does not seem to work properly on win32

			const Tchar* Title = RE_T("Window");
			int32 Width = 800;
			int32 Height = 600;
			
			int32 Style = WINDOWSTYLE_STANDARD_WINDOW;
			int32 Flags = WINDOW_FLAG_NONE;

			//TODO: Create Application class to retrive desktop size so that 
			//user can use that to retrive sixe and position were he wants
			
			//Position - '-1' indicates in the middle

			int32 x = -1;
			int32 y = -1;
			//Color
			Math::Color BackgroundColor;

			Bitmap Icon;
			Bitmap Cursor;

			int32 CursorHotspotX = 0;
			int32 CursorHotspotY = 0;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Class for platform implementation of a window
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		class RE_API IWindowImpl
		{
			RE_INTERFACE(IWindowImpl);

		public:
			IWindowImpl() {}
			virtual ~IWindowImpl() {}

			//Create platform handles for a window
			virtual bool Create(const WindowDesc& desc) = 0;
			//Show window
			virtual void Show() const = 0;
			//Peeks all the events of the OS and pushes them onto the eventqueue
			virtual void PeekEvents() = 0;
			//Pushes a new event onto the eventqueue
			virtual void PushEvent(const Event& pEvent) = 0;
			//Pops the top event off the eventqueue - returns true if an event got poped
			virtual bool PopEvent(Event& pEvent) = 0;
			//Blocks until an event occurs
			virtual void GetEvent(Event& pEvent) = 0;
			//Sends a quit message to the operatingsystem via the window
			virtual void SendQuitEvent(int32 exitCode) const = 0;
			//Set Cursor for the window
			virtual void SetCursor(const Bitmap& cursor, RayEngine::int32 hotspotX, RayEngine::int32 hotspotY) = 0;
			//Set Icon for the window
			virtual void SetIcon(const Bitmap& icon) = 0;
			//Set color for the background
			virtual void SetBackground(uint8 r, uint8 g, uint8 b) = 0;
			virtual void SetBackground(const Math::Color& color) = 0;
			//Returns a new instance of IWindowImple that is equal to to the current one. If called a new 
			//window instance will be created on the OS
			virtual IWindowImpl* Copy() const = 0;
			//Get titlebar text
			virtual const Tchar* GetTitle() const = 0;
			//Get size
			virtual int32 GetWidth() const = 0;
			virtual int32 GetHeight() const = 0;
			//Get struct that describes the window
			virtual void GetInfo(WindowDesc& desc) const = 0;
			//Returns the OS-Handle to a window
			virtual NativeWindowHandle GetNativeHandle() const = 0;
		};



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Class for to represent a main window - Can be seen as a wrapper for IWindowImpl
		class RE_API Window
		{
		public:
			Window(const WindowDesc& desc);
			Window(Window&& other);
			Window(const Window& other);
			~Window();

			//Show window
			//TODO: Show normal, minimized or maximized
			void Show() const;
			
			//TODO: Functions for minimizing and maximizing a window

			//Checks the eventqueue for events. Does not block the thread - Returns true if there were an event
			bool PeekEvent(Event& pEvent) const;
			//Checks the eventqueue for events. Blocks the thread until an event is poped from the eventqueue
			void GetEvent(Event& pEvent) const;
			//Send Quit even
			void SendQuitEvent(int32 exitCode) const;
			//Set Cursor for the window
			void SetCursor(const Bitmap& cursor, RayEngine::int32 hotspotX, RayEngine::int32 hotspotY);
			//Set Icon for the window
			void SetIcon(const Bitmap& icon);
			//Set background color
			void SetBackground(uint8 r, uint8 g, uint8 b);
			void SetBackground(const Math::Color& color);
			//Get clientsize
			int32 GetWidth() const;
			int32 GetHeight() const;
			const Tchar* GetTitle() const;
			//Get implementation
			const IWindowImpl* GetImplementation() const;

			NativeWindowHandle GetNativeHandle() const;

			//Get struct that describes the window
			void GetInfo(WindowDesc& desc) const;

			//Operators for assignment
			Window& operator=(const Window& other);
			Window& operator=(Window&& other);

		private:
			//Pointer to implementation
			IWindowImpl* m_Impl;
		};
	}
}