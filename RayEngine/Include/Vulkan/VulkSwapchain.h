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
#include "..\Graphics\ISwapchain.h"
#include "VulkTexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class VulkDevice;
		class VulkDeviceContext;
		class VulkFactory;



		/////////////////////////////////////////////////////////////
		class VulkSwapchain : public ISwapchain
		{
		public:
			VulkSwapchain(const VulkSwapchain& other) = delete;
			VulkSwapchain& operator=(const VulkSwapchain& other) = delete;
			VulkSwapchain(VulkSwapchain&& other) = delete;
			VulkSwapchain& operator=(VulkSwapchain&& other) = delete;

		public:
			VulkSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info);
			~VulkSwapchain();

			VkSurfaceKHR GetVkSurfaceKHR() const;
			
			VkSwapchainKHR GetVkSwapchainKHR() const;

			void Resize(int32 width, int32 height) override final;

			void Present() const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void ReleaseObjects();
			
			void Create(const SwapchainInfo& info);

		private:
			static VkExtent2D GetSupportedSize(const VkSurfaceCapabilitiesKHR& capabilities, int32 width, int32 height);
			
			static VkSurfaceFormatKHR GetSupportedFormat(VulkDevice* pDevice, VkSurfaceKHR surface, VkFormat desiredFormat);

		private:
			VulkDevice* m_Device;
			VulkFactory* m_Factory;
			VulkDeviceContext* m_CommandQueue;
			
			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;
			
			std::vector<VulkTexture*> m_Textures;
			
			IObject::CounterType m_References;
		};
	}
}