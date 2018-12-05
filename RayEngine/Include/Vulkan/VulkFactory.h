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
/*#include "..\Graphics\IFactory.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkFactory final : public IFactory
		{
			RE_IMPLEMENT_INTERFACE(VulkFactory);

		public:
			VulkFactory(bool debugLayers);
			~VulkFactory();

			inline VkInstance GetVkInstance() const
			{
				return m_Instance;
			}

			//void EnumerateAdapters(AdapterList* pList) const override final;
			
			bool CreateDevice(IDevice** ppDevice, const DeviceDesc* pDesc) override final;
			
			bool CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainDesc* pDesc) override final;
			
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceDesc* pDeviceDesc, ISwapchain** ppSwapchain, const SwapchainDesc* pSwapchainDesc) override final;

			void SetName(const std::string& name) override final;
			
			GRAPHICS_API GetGraphicsApi() const override final;
			
			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(bool debugLayers);

		private:
			VkInstance m_Instance;
			VkDebugReportCallbackEXT m_DbgCallback;
		
			IObject::CounterType m_References;

		public:
			static bool InstanceLayersSupported(const char* const * ppNeededLayers, int32 count);
			
			static bool InstanceExtensionsSupported(const char* const * ppNeededExtensions, int32 count);
			
			static bool DeviceExtensionsSupported(VkPhysicalDevice adapter, const char* const * ppNeededExtensions, int32 count);
			
			static void FillAdapterDesc(AdapterDesc* pDesc, VkPhysicalDeviceFeatures* pFeatures, VkPhysicalDeviceProperties* pProperties, int32 id, int32 supportFlags);
			
			static void CheckQueueFamilySupport(VkPhysicalDevice* pAdapter, VkQueueFamilyProperties* pQueuefamily, int32* pSupportFlags);

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, 
				uint64_t obj, size_t location, int32_t code, const char* pLayerPrefix, const char* pMsg, void* pUserData);
		};
	}
}*/