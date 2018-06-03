#pragma once

#include "Event.h"
#include "Icon.h"
#include "Cursor.h"

namespace RayEngine
{
	namespace System
	{
		enum WINDOWSTYLE
		{
			WINDOWSTYLE_UNKNOWN = 0,
			WINDOWSTYLE_BORDERLESS = 1,
			WINDOWSTYLE_FULLSCREEN = 2,
			WINDOWSTYLE_WINDOW = 3,
		};

		//Struct to decribe a window
		struct WindowDesc
		{
			const Tchar* Title = RE_T("Window");
			int32 Width = 800;
			int32 Height = 600;
			int32 Style = WINDOWSTYLE_BORDERLESS;
			//Position - '-1' indicates in the middle

			//TODO: Create Application class to retrive desktop size so that 
			//user can use that to retrive sixe and position were he wants
			int32 x = -1;
			int32 y = -1;
			//Color
			struct
			{
				uint8 r = 0;
				uint8 g = 0;
				uint8 b = 0;
			} Color;

			//TODO: Have Bitmap class for both cursor and icon
			Icon icon;
			Cursor cursor;
		};

		//Class for platform implementation of a window
		class IWindowImpl
		{
		public:
			IWindowImpl(const IWindowImpl& other) = delete;
			IWindowImpl(IWindowImpl&& other) = delete;
			IWindowImpl& operator=(const IWindowImpl& other) = delete;
			IWindowImpl& operator=(IWindowImpl&& other) = delete;
			
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
			virtual void SetCursor(const Cursor& cursor) = 0;
			//Set Icon for the window
			virtual void SetIcon(const Icon& icon) = 0;
			//Returns a new instance of IWindowImple that is equal to to the current one. If called a new 
			//window instance will be created on the OS
			virtual IWindowImpl* Copy() const = 0;
			//Returns a new instacne of a IWindowImple that has the ownership of all native handles of 'this'.
			//If called on an instance the instance will be invalid
			virtual IWindowImpl* Move() = 0;
			//Get titlebar text
			virtual const Tchar* GetTitle() const = 0;
			//Get size
			virtual int32 GetWidth() const = 0;
			virtual int32 GetHeight() const = 0;
			//Get struct that describes the window
			virtual void GetDesc(WindowDesc& desc) const = 0;
		};

		//Class for to represent a main window - Can be seen as a wrapper for IWindowImpl
		class Window
		{
		public:
			Window(const WindowDesc& desc);
			Window(const Window& other);
			Window(Window&& other);
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
			void SetCursor(const Cursor& cursor);
			//Set Icon for the window
			void SetIcon(const Icon& icon);
			//Get clientsize
			int32 GetWidth() const;
			int32 GetHeight() const;
			const Tchar* GetTitle() const;
			//Get implementation
			const IWindowImpl* GetImplementation() const;
			//Get struct that describes the window
			void GetDesc(WindowDesc& desc) const;
			//Operators for assignment
			Window& operator=(const Window& other);
			Window& operator=(Window&& other);

		private:
			//Pointer to implementation
			IWindowImpl* m_Impl;
		};
	}
}