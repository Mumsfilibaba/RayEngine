#include "..\..\Include\System\Log.h"
#include "..\..\Include\Vulkan\VkFactory.h"
#include "..\..\Include\Vulkan\VKDevice.h"
#include <vector>

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
		/////////////////////////////////////////////////////////////
		VKFactory::VKFactory(bool debugLayers)
			: m_Instance(nullptr),
			m_DbgCallback(0)
		{
			Create(debugLayers);
		}



		/////////////////////////////////////////////////////////////
		VKFactory::VKFactory(VKFactory&& other)
			: m_Instance(other.m_Instance),
			m_DbgCallback(other.m_DbgCallback)
		{
			other.m_Instance = nullptr;
			other.m_DbgCallback = 0;
		}



		/////////////////////////////////////////////////////////////
		VKFactory::~VKFactory()
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
		}



		/////////////////////////////////////////////////////////////
		void VKFactory::Create(bool debugLayers)
		{
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
				return;


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
				return;
			

			if (debugLayers)
			{
				//Setup debug callback
				VkDebugReportCallbackCreateInfoEXT dbgCallbackInfo = {};
				dbgCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				dbgCallbackInfo.pfnCallback = VKFactory::DebugCallback;
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

			return;
		}



		/////////////////////////////////////////////////////////////
		void VKFactory::EnumerateAdapters(AdapterList& list) const
		{
			uint32 deviceCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
			if (result != VK_SUCCESS || deviceCount == 0)
				return;


			list = AdapterList(deviceCount);


			std::vector<VkPhysicalDevice> devices;
			devices.resize(list.Count);
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


			for (int32 i = 0; i < list.Count; i++)
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
					CheckQueueFamilySupport(devices[i], queueFamilies[j], queueSupport);

					if (queueSupport > highestQueueSupport)
						highestQueueSupport = queueSupport;
				}

				supportFlags |= highestQueueSupport;


				FillAdapterInfo(list[i], features, properties, i, supportFlags);
			}
		}



		/////////////////////////////////////////////////////////////
		bool VKFactory::CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const
		{
			return ((*device) = new VKDevice(m_Instance, deviceInfo)) != nullptr;
		}



		/////////////////////////////////////////////////////////////
		bool VKFactory::CreateSwapchain(ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const
		{
			*swapchain = nullptr;

			//Not implemented for now since vulkan needs a device
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKFactory::CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo, ISwapchain** swapchain, 
			const SwapchainInfo& swapchainInfo) const
		{
			VkSurfaceKHR surface = 0;
			VkResult result = VulkanCreateSwapchainSurface(m_Instance, &surface, swapchainInfo.Window->GetImplementation());
			if (result != VK_SUCCESS)
				return false;
			
			(*device) = new VKDevice(m_Instance, deviceInfo);
			VKDevice* d = reinterpret_cast<VKDevice*>(*device);
			
			VKSwapchain* sc = d->CreateVKSwapchain(surface);
			(*swapchain) = sc;

			return sc != nullptr;
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API VKFactory::GetGraphicsApi() const
		{
			return GRAPHICS_API_VULKAN;
		}



		/////////////////////////////////////////////////////////////
		VKFactory& VKFactory::operator=(VKFactory&& other)
		{
			if (this != &other)
			{
				m_Instance = other.m_Instance;
				m_DbgCallback = other.m_DbgCallback;

				other.m_Instance = nullptr;
				other.m_DbgCallback = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		bool VKFactory::InstanceLayersSupported(const char * const * neededLayers, int32 count)
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



		/////////////////////////////////////////////////////////////
		bool VKFactory::InstanceExtensionsSupported(const char* const * neededExtensions, int32 count)
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



		/////////////////////////////////////////////////////////////
		bool VKFactory::DeviceExtensionsSupported(VkPhysicalDevice& adapter, const char * const * neededExtensions, int32 count)
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



		/////////////////////////////////////////////////////////////
		void VKFactory::FillAdapterInfo(AdapterInfo& info, VkPhysicalDeviceFeatures& features, 
			VkPhysicalDeviceProperties& properties, int32 id, int32 supportFlags)
		{
			info.ApiID = id;
			info.VendorID = properties.vendorID;
			info.DeviceID = properties.deviceID;


			info.ModelName = properties.deviceName;
			info.VendorName = AdapterInfo::GetVendorString(properties.vendorID);


			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
				supportFlags |= ADAPTER_FLAGS_SOFTWARE;

			if (features.geometryShader)
				supportFlags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			if (features.tessellationShader)
				supportFlags |= ADAPTER_FLAGS_TESSELATIONSHADERS;


			info.Limits.Texture1D.Width = properties.limits.maxImageDimension1D;

			info.Limits.Texture2D.Width = properties.limits.maxImageDimension2D;
			info.Limits.Texture2D.Height = properties.limits.maxImageDimension2D;

			info.Limits.Texture3D.Width = properties.limits.maxImageDimension3D;
			info.Limits.Texture3D.Height = properties.limits.maxImageDimension3D;
			info.Limits.Texture3D.Depth = properties.limits.maxImageDimension3D;

			info.Limits.TextureCube.Width = properties.limits.maxImageDimensionCube;
			info.Limits.TextureCube.Height = properties.limits.maxImageDimensionCube;

			info.Limits.RenderTargetCount = properties.limits.maxColorAttachments;

			info.Flags = supportFlags;
		}



		/////////////////////////////////////////////////////////////
		void VKFactory::CheckQueueFamilySupport(VkPhysicalDevice& adapter, VkQueueFamilyProperties& queuefamily, int32& supportFlags)
		{
			if (queuefamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				supportFlags |= ADAPTER_FLAGS_GRAPHICS;
			if (queuefamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				supportFlags |= ADAPTER_FLAGS_COMPUTE;
		}



		/////////////////////////////////////////////////////////////
		VKAPI_ATTR VkBool32 VKAPI_CALL VKFactory::DebugCallback(VkDebugReportFlagsEXT flags, 
			VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
			const char* layerPrefix, const char* msg, void* userData)
		{
			System::Log log;
			System::LOG_SEVERITY severity = System::LOG_SEVERITY_UNKNOWN;

			if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
				severity = System::LOG_SEVERITY_ERROR;
			else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT || flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
				severity = System::LOG_SEVERITY_INFO;
			else
				severity = System::LOG_SEVERITY_UNKNOWN;

			log.Write(severity, msg);
			return VK_FALSE;
		}
	}
}