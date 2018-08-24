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
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkFactory : public IFactory
		{
		public:
			VulkFactory(const VulkFactory& other) = delete;
			VulkFactory& operator=(const VulkFactory& other) = delete;
			VulkFactory(VulkFactory&& other) = delete;
			VulkFactory& operator=(VulkFactory&& other) = delete;

		public:
			VulkFactory(bool debugLayers);
			~VulkFactory();

			VkInstance GetVkInstance() const;

			void EnumerateAdapters(AdapterList& list) const override final;
			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo,
				ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;

			void SetName(const std::string& name) override final;
			GRAPHICS_API GetGraphicsApi() const override final;

		private:
			void Create(bool debugLayers);

		private:
			VkInstance m_Instance;
			VkDebugReportCallbackEXT m_DbgCallback;

		public:
			static bool InstanceLayersSupported(const char* const * ppNeededLayers, int32 count);
			static bool InstanceExtensionsSupported(const char* const * ppNeededExtensions, int32 count);
			static bool DeviceExtensionsSupported(VkPhysicalDevice& adapter, const char* const * ppNeededExtensions, int32 count);
			static void FillAdapterInfo(AdapterInfo& info, VkPhysicalDeviceFeatures& features,
				VkPhysicalDeviceProperties& properties, int32 id, int32 supportFlags);
			static void CheckQueueFamilySupport(VkPhysicalDevice& adapter, VkQueueFamilyProperties& queuefamily, int32& supportFlags);

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
				int32_t code, const char* pLayerPrefix, const char* pMsg, void* pUserData);
		};
	}
}