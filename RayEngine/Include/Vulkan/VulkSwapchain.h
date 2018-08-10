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

			int32 GetCurrentBuffer() const override final;
			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;
			void QueryFactory(IFactory** ppFactory) const override final;
			void QueryCommandQueue(ICommandQueue** ppCommandQueue) const override final;

		private:
			void ReleaseObjects();
			void Create(const SwapchainInfo& info);

		private:
			static VkExtent2D GetSupportedSize(const VkSurfaceCapabilitiesKHR& capabilities, int32 width, int32 height);
			static VkSurfaceFormatKHR GetSupportedFormat(VulkDevice* pDevice, VkSurfaceKHR surface, VkFormat desiredFormat);

		private:
			VulkDevice* m_Device;
			VulkFactory* m_Factory;
			ICommandQueue* m_CommandQueue;
			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;
			std::vector<VulkTexture*> m_Textures;
		};
	}
}