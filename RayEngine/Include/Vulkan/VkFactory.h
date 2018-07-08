#pragma once

#include "..\Graphics\IFactory.h"
#include "VkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VKFactory : public IFactory
		{
		public:
			VKFactory(const VKFactory& other) = delete;
			VKFactory& operator=(const VKFactory& other) = delete;

		public:
			VKFactory(bool debugLayers);
			VKFactory(VKFactory&& other);
			~VKFactory();

			void EnumerateAdapters(AdapterInfo** adapters, int32& count) const override final;

			bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const override final;
			bool CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const override final;

			void DestroySwapchain(const IDevice* const device, ISwapchain** swapchain) const override final;

			GRAPHICS_API GetGraphicsApi() const override final;
			
			VKFactory& operator=(VKFactory&& other);

		private:
			void Create(bool debugLayers);

		private:
			VkInstance m_Instance;
			VkDebugReportCallbackEXT m_DbgCallback;

		public:
			static bool InstanceLayersSupported(const char* const * neededLayers, int32 count);
			static bool InstanceExtensionsSupported(const char* const * neededExtensions, int32 count);
			static bool DeviceExtensionsSupported(VkPhysicalDevice& adapter, const char* const * neededExtensions, int32 count);
			static void FillAdapterInfo(AdapterInfo& info, VkPhysicalDeviceFeatures& features,
				VkPhysicalDeviceProperties& properties, int32 id, int32 supportFlags);
			static void CheckQueueFamilySupport(VkPhysicalDevice& adapter, VkQueueFamilyProperties& queuefamily, int32& supportFlags);

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
				int32_t code, const char* layerPrefix, const char* msg, void* userData);
		};
	}
}