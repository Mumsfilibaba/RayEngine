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

#include "..\..\Include\OpenGL\GLSwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLSwapchain::GLSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info)
			: m_References(0)
		{
		}



		/////////////////////////////////////////////////////////////
		GLSwapchain::~GLSwapchain()
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::Resize(int32 width, int32 height)
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::Present() const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::QueryDevice(IDevice** ppDevice) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::QueryFactory(IFactory** ppFactory) const
		{
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLSwapchain::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::Create(const SwapchainInfo& info)
		{
		}



		/////////////////////////////////////////////////////////////
		void GLSwapchain::CreateTextures(const SwapchainInfo& info)
		{
		}
	}
}