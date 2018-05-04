#pragma once

#include "Event.h"

namespace RayEngine
{
	namespace System
	{
		class IWindowImpl
		{
		public:
			IWindowImpl(const IWindowImpl& other) = delete;
			IWindowImpl(IWindowImpl&& other) = delete;
			IWindowImpl& operator=(const IWindowImpl& other) = delete;
			IWindowImpl& operator=(IWindowImpl&& other) = delete;
			
			virtual ~IWindowImpl() {}

			//Peeks all the events of the OS and pushes them onto the eventqueue
			virtual void PeekEvents() = 0;
			//Pushes a new event onto the eventqueue
			virtual void PushEvent(const Event& pEvent) = 0;
			//Pops the top event off the eventqueue - returns true if an event got poped
			virtual bool PopEvent(Event& pEvent) = 0;
			//Blocks until an event occurs
			virtual void GetEvent(Event& pEvent) = 0;
		};

		//Class for to represent a main window 
		class Window
		{
		public:
			Window(const TChar* = TC("Window"), int32 width = 800, int32 height = 600);
			Window(const Window& other);
			Window(Window&& other);
			~Window();

			//Checks the eventqueue for events. Does not block the thread - Returns true if there were an event
			bool PeekEvent(Event& pEvent) const;
			//Checks the eventqueue for events. Blocks the thread until an event is poped from the eventqueue
			void GetEvent(Event& pEvent) const;

			//Get clientsize
			int32 GetWidth() const;
			int32 GetHeight() const;
			const TChar* GetTitle() const;

			//Get implementation
			const IWindowImpl* GetImplementation() const;

			//Operators for assignment
			Window& operator=(const Window& other);
			Window& operator=(Window&& other);

		private:
			//Pointer to implementation
			IWindowImpl* m_Impl;
			//Client Size
			int32 m_Width;
			int32 m_Height;
		};
	}
}