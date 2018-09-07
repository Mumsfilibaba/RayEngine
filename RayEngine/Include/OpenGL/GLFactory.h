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

#include "..\Graphics\IFactory.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class GLFactory final : public IFactory
		{
		public:
			GLFactory(const GLFactory& other) = delete;
			GLFactory& operator=(const GLFactory& other) = delete;
			GLFactory(GLFactory&& other) = delete;
			GLFactory& operator=(GLFactory&& other) = delete;

		public:
			GLFactory(bool debugLayer);
			~GLFactory();
			
			void EnumerateAdapters(AdapterList& list) const override final;
			
			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			
			bool CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo) override final;
			
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;

			void SetName(const std::string& name) override final;
			
			GRAPHICS_API GetGraphicsApi() const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(bool debugLayer);

		private:
			IObject::CounterType m_References;
		};
	}
}