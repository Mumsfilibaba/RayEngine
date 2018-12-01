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

#include <vector>
#include "../../Include/Debug/Debug.h"
#include "../../Include/Vulkan/VulkFactory.h"
#include "../../Include/Vulkan/VulKDevice.h"

#define VULKAN_EXTENSION_COUNT 3
#if defined(RE_PLATFORM_ANDROID)
#define VULKAN_LAYER_COUNT 6
#else
#define VULKAN_LAYER_COUNT 1
#endif

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkFactory::VulkFactory(bool debugLayers)
			: m_Instance(nullptr),
			m_DbgCallback(0),
			m_References(0)
		{
			AddRef();

			Create(debugLayers);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkFactory::~VulkFactory()
		{
			if (m_DbgCallback != 0)
			{
				vkDestroyDebugReportCallbackEXT(m_Instance, m_DbgCallback, nullptr);
				m_DbgCallback = 0;
			}

			if (m_Instance != nullptr)
			{
				vkDestroyInstance(m_Instance, nullptr);
				m_Instance = nullptr;
			}

			//LogService::GraphicsLog(nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkFactory::Create(bool debugLayers)
		{
			/*if (debugLayers)
				LogService::GraphicsLog(new OutputLog());
			else
				LogService::GraphicsLog(new NullLog());*/


			VkApplicationInfo aInfo = {};
			aInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			aInfo.apiVersion = VK_API_VERSION_1_0;
			aInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			aInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			aInfo.pApplicationName = "TestApplication";
			aInfo.pEngineName = "RayEngine";
			aInfo.pNext = nullptr;


			const char* neededExtensions[VULKAN_EXTENSION_COUNT] =
			{
#ifdef RE_DEBUG
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
#if defined (RE_PLATFORM_WINDOWS)
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
				VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#endif
				VK_KHR_SURFACE_EXTENSION_NAME,
			};

			if (!InstanceExtensionsSupported(neededExtensions, VULKAN_EXTENSION_COUNT))
			{
				//LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Needed extensions not supported.");
				return;
			}


			VkInstanceCreateInfo iInfo = {};
			iInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			iInfo.enabledExtensionCount = VULKAN_EXTENSION_COUNT;
			iInfo.ppEnabledExtensionNames = neededExtensions;
			iInfo.flags = 0;
			iInfo.pApplicationInfo = &aInfo;
			iInfo.pNext = nullptr;
			
			if (debugLayers)
			{
				const char* neededLayers[VULKAN_LAYER_COUNT] =
				{
#if defined(RE_PLATFORM_ANDROID)
					"VK_LAYER_GOOGLE_threading",
					"VK_LAYER_LUNARG_parameter_validation",
					"VK_LAYER_LUNARG_object_tracker",
					"VK_LAYER_LUNARG_core_validation",
					"VK_LAYER_LUNARG_swapchain",
					"VK_LAYER_GOOGLE_unique_objects"
#else
					"VK_LAYER_LUNARG_standard_validation",
#endif
				};

				if (debugLayers)
				{
					if (!InstanceLayersSupported(neededLayers, VULKAN_LAYER_COUNT))
						return;
				}

				iInfo.enabledLayerCount = VULKAN_LAYER_COUNT;
				iInfo.ppEnabledLayerNames = neededLayers;
			}
			else
			{
				iInfo.enabledLayerCount = 0;
				iInfo.ppEnabledLayerNames = nullptr;
			}

			VkResult result = vkCreateInstance(&iInfo, nullptr, &m_Instance);
			if (result != VK_SUCCESS)
			{
				//LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Failed to create instance.");
				return;
			}
			

			if (debugLayers)
			{
				//Setup debug callback
				VkDebugReportCallbackCreateInfoEXT dbgCallbackInfo = {};
				dbgCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				dbgCallbackInfo.pfnCallback = VulkFactory::DebugCallback;
				dbgCallbackInfo.pNext = nullptr;
				dbgCallbackInfo.pUserData = nullptr;
				dbgCallbackInfo.flags = 
					VK_DEBUG_REPORT_ERROR_BIT_EXT | 
					VK_DEBUG_REPORT_WARNING_BIT_EXT |
					VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | 
					VK_DEBUG_REPORT_DEBUG_BIT_EXT | 
					VK_DEBUG_REPORT_INFORMATION_BIT_EXT;

				result = vkCreateDebugReportCallbackEXT(m_Instance, &dbgCallbackInfo, nullptr, &m_DbgCallback);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkFactory::EnumerateAdapters(AdapterList* pList) const
		{
			uint32 deviceCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
			if (result != VK_SUCCESS || deviceCount == 0)
				return;


			(*pList) = AdapterList(deviceCount);


			std::vector<VkPhysicalDevice> devices;
			devices.resize(pList->Count);
			result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
			if (result != VK_SUCCESS)
				return;


			VkPhysicalDeviceFeatures features;
			VkPhysicalDeviceProperties properties;
			std::vector<VkQueueFamilyProperties> queueFamilies;
			int32 supportFlags = 0;

			const char* deviceExtensions[] =
			{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			};


			for (int32 i = 0; i < pList->Count; i++)
			{
				vkGetPhysicalDeviceFeatures(devices[i], &features);
				vkGetPhysicalDeviceProperties(devices[i], &properties);

				if (DeviceExtensionsSupported(devices[i], deviceExtensions, 1))
				{
					supportFlags |= ADAPTER_FLAGS_SWAPCHAIN;
				}


				uint32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, nullptr);

				queueFamilies.resize(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies.data());


				int32 queueSupport = 0;
				int32 highestQueueSupport = 0;
				for (int32 j = 0; j < static_cast<int32>(queueFamilies.size()); j++)
				{
					CheckQueueFamilySupport(&devices[i], &queueFamilies[j], &queueSupport);

					if (queueSupport > highestQueueSupport)
						highestQueueSupport = queueSupport;
				}

				supportFlags |= highestQueueSupport;


				FillAdapterDesc(&(*pList)[i], &features, &properties, i, supportFlags);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::CreateDevice(IDevice** ppDevice, const DeviceDesc* pDesc)
		{
			return ((*ppDevice) = new VulkDevice(pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainDesc* pDesc)
		{
			*ppSwapchain = nullptr;

			//Not implemented for now since vulkan needs a device
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceDesc* pDeviceDesc, ISwapchain** ppSwapchain, const SwapchainDesc* pSwapchainDesc)
		{	
			VulkDevice* pVulkDevice = new VulkDevice(pDeviceDesc);
			(*ppDevice) = pVulkDevice;
			
			VulkSwapchain* pVulkSwapchain = new VulkSwapchain(pVulkDevice, pSwapchainDesc);
			(*ppSwapchain) = pVulkSwapchain;

			return pVulkSwapchain != nullptr;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkFactory::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GRAPHICS_API VulkFactory::GetGraphicsApi() const
		{
			return GRAPHICS_API_VULKAN;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkFactory::GetReferenceCount() const
		{
			return m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkFactory::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkFactory::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::InstanceLayersSupported(const char * const * neededLayers, int32 count)
		{
			uint32 layerCount = 0;
			VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			if (result != VK_SUCCESS)
				return false;


			std::vector<VkLayerProperties> availableLayers;
			availableLayers.resize(layerCount);
			result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
			if (result != VK_SUCCESS)
				return false;


			bool found = false;
			std::vector<VkLayerProperties> layers;
			layers.resize(count);

			for (int32 i = 0; i < count; i++)
			{
				found = false;
				for (int32 j = 0; j < static_cast<int32>(availableLayers.size()); j++)
				{
					if (strcmp(neededLayers[i], availableLayers[j].layerName) == 0)
					{
						layers[i] = availableLayers[j];
						found = true;
						break;
					}
				}

				if (found == false)
					return false;
			}

			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::InstanceExtensionsSupported(const char* const * neededExtensions, int32 count)
		{
			uint32 extensionCount = 0;
			VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			if (result != VK_SUCCESS)
				return false;


			std::vector<VkExtensionProperties> availableExtensions;
			availableExtensions.resize(extensionCount);
			result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
			if (result != VK_SUCCESS)
				return false;


			bool found = false;
			std::vector<VkExtensionProperties> extensions;
			extensions.resize(count);

			for (int32 i = 0; i < count; i++)
			{
				found = false;
				for (int32 j = 0; j < static_cast<int32>(availableExtensions.size()); j++)
				{
					if (strcmp(neededExtensions[i], availableExtensions[j].extensionName) == 0)
					{
						extensions[i] = availableExtensions[j];
						found = true;
						break;
					}
				}

				if (found == false)
					return false;
			}

			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkFactory::DeviceExtensionsSupported(VkPhysicalDevice adapter, const char * const * neededExtensions, int32 count)
		{
			uint32 extensionCount = 0;
			VkResult result = vkEnumerateDeviceExtensionProperties(adapter, nullptr, &extensionCount, nullptr);
			if (result != VK_SUCCESS)
				return false;


			std::vector<VkExtensionProperties> availableExtensions;
			availableExtensions.resize(extensionCount);

			result = vkEnumerateDeviceExtensionProperties(adapter, nullptr, &extensionCount, availableExtensions.data());
			if (result != VK_SUCCESS)
				return false;


			bool supported = false;
			for (int32 i = 0; i < count; i++)
			{
				supported = false;
				for (int32 j = 0; j < static_cast<int32>(availableExtensions.size()); j++)
				{
					if (strcmp(neededExtensions[i], availableExtensions[j].extensionName) == 0)
					{
						supported = true;
						break;
					}
				}

				if (supported == false)
					return false;
			}
			
			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkFactory::FillAdapterDesc(AdapterDesc* pDesc, VkPhysicalDeviceFeatures* pFeatures, VkPhysicalDeviceProperties* pProperties, int32 id, int32 supportFlags)
		{
			pDesc->ApiID = id;
			pDesc->VendorID = pProperties->vendorID;
			pDesc->DeviceID = pProperties->deviceID;


			pDesc->ModelName = pProperties->deviceName;
			pDesc->VendorName = AdapterDesc::GetVendorString(pProperties->vendorID);


			if (pProperties->deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
				supportFlags |= ADAPTER_FLAGS_SOFTWARE;

			if (pFeatures->geometryShader)
				supportFlags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			if (pFeatures->tessellationShader)
				supportFlags |= ADAPTER_FLAGS_TESSELATIONSHADERS;


			pDesc->Limits.Texture1D.Width = pProperties->limits.maxImageDimension1D;

			pDesc->Limits.Texture2D.Width = pProperties->limits.maxImageDimension2D;
			pDesc->Limits.Texture2D.Height = pProperties->limits.maxImageDimension2D;

			pDesc->Limits.Texture3D.Width = pProperties->limits.maxImageDimension3D;
			pDesc->Limits.Texture3D.Height = pProperties->limits.maxImageDimension3D;
			pDesc->Limits.Texture3D.Depth = pProperties->limits.maxImageDimension3D;

			pDesc->Limits.TextureCube.Width = pProperties->limits.maxImageDimensionCube;
			pDesc->Limits.TextureCube.Height = pProperties->limits.maxImageDimensionCube;

			pDesc->Limits.RenderTargetCount = pProperties->limits.maxColorAttachments;

			pDesc->Flags = supportFlags;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkFactory::CheckQueueFamilySupport(VkPhysicalDevice* pAdapter, VkQueueFamilyProperties* pQueuefamily, int32* pSupportFlags)
		{
			if (pQueuefamily->queueFlags & VK_QUEUE_GRAPHICS_BIT)
				*pSupportFlags |= ADAPTER_FLAGS_GRAPHICS;
			if (pQueuefamily->queueFlags & VK_QUEUE_COMPUTE_BIT)
				*pSupportFlags |= ADAPTER_FLAGS_COMPUTE;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VKAPI_ATTR VkBool32 VKAPI_CALL VulkFactory::DebugCallback(VkDebugReportFlagsEXT flags, 
			VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
			const char* layerPrefix, const char* msg, void* userData)
		{
			LOG_SEVERITY severity = LOG_SEVERITY_UNKNOWN;

			if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
				severity = LOG_SEVERITY_ERROR;
			else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT || flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
				severity = LOG_SEVERITY_INFO;
			else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
				severity = LOG_SEVERITY_WARNING;
			else
				severity = LOG_SEVERITY_UNKNOWN;

			Debug::Log(severity, msg);
			return VK_FALSE;
		}
	}
}