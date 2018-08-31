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
#include <vector>
#include "..\..\Include\Graphics\ISwapchain.h"
#include "GLCommon.h"
#include "GLDeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class GLFactory;
		class GLDevice;
		class GLDeviceContext;
		class GLTexture;



		/////////////////////////////////////////////////////////////
		class GLSwapchain final : public ISwapchain
		{
		public:
			GLSwapchain(const GLSwapchain& other) = delete;
			GLSwapchain& operator=(const GLSwapchain& other) = delete;
			GLSwapchain(GLSwapchain&& other) = delete;
			GLSwapchain& operator=(GLSwapchain&& other) = delete;

		public:
			GLSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info);
			~GLSwapchain();

			int32 GetCurrentBuffer() const override final;
			void Present() const override final;

			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;

			void SetName(const std::string& name) override final;
			void QueryDevice(IDevice** ppDevice) const override final;
			void QueryFactory(IFactory** ppFactory) const override final;

		private:
			void Create(const SwapchainInfo& info);
			void CreateTextures(const SwapchainInfo& info);

		private:
			GLDevice * m_Device;
			GLFactory* m_Factory;
			GLTexture* m_Texture;
			int32 m_BufferCount;
			mutable int32 m_CurrentBuffer;
		};
	}
}