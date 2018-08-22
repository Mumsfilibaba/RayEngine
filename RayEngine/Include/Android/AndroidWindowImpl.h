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
#include "..\System\Window.h"

#if defined(RE_PLATFORM_ANDROID)

#include <queue>
#include <android/native_window.h>

namespace RayEngine
{
	namespace System
	{
		class AndroidWindowImpl : public IWindowImpl
		{
		public:
			AndroidWindowImpl(const AndroidWindowImpl& other) = delete;
			AndroidWindowImpl(AndroidWindowImpl&& other) = delete;
			AndroidWindowImpl& operator=(const AndroidWindowImpl& other) = delete;
			AndroidWindowImpl& operator=(AndroidWindowImpl&& other) = delete;

		public:
			AndroidWindowImpl();
			~AndroidWindowImpl();

			bool Create(const WindowInfo& desc) override final;

			void Show() const override final;

			void PeekEvents() override final;
			void PushEvent(const Event& event) override final;
			bool PopEvent(Event& event) override final;
			void GetEvent(Event& event) override final;
			void SendQuitEvent(int32 exitCode) const override final;

			void SetCursor(const Bitmap& cursor, int32 hotspotX, int32 hotspotY) override final;
			void SetIcon(const Bitmap& icon) override final;
			void SetBackground(const Math::Color& color) override final;
			void SetBackground(uint8 r, uint8 g, uint8 b) override final;

			IWindowImpl* Copy() const override final;

			const Tchar* GetTitle() const override final;

			virtual int32 GetWidth() const override final;
			virtual int32 GetHeight() const override final;

			void GetDesc(WindowInfo& desc) const override final;

		private:
			std::queue<Event> m_Events;
			int32 m_Width;
			int32 m_Height;
			int32 m_Color;
			int32 m_Flags;
		};
	}
}

#endif