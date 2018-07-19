#include "..\..\Include\Vulkan\VKDevice.h"
#include <vector>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VKDevice::VKDevice(VkInstance instance, const DeviceInfo& deviceInfo)
			: m_Device(nullptr),
			m_Adapter(nullptr)
		{
			Create(instance, deviceInfo);
		}



		/////////////////////////////////////////////////////////////
		VKDevice::VKDevice(VKDevice&& other)
			: m_Device(other.m_Device),
			m_Adapter(other.m_Adapter)
		{
			other.m_Device = nullptr;
			other.m_Adapter = nullptr;
		}



		/////////////////////////////////////////////////////////////
		VKDevice::~VKDevice()
		{
			if (m_Device != nullptr)
			{
				vkDestroyDevice(m_Device, nullptr);
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateFence(IFence** fence) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateShader(IShader** shader, const ShaderByteCode& byteCode) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateRenderTargetView(IRenderTargetView** view, const RenderTargetViewInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateDepthStencilView(IDepthStencilView** view, const DepthStencilViewInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateTexture(ITexture** texture, const TextureInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VKDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		VKDevice& VKDevice::operator=(VKDevice&& other)
		{
			if (this != &other)
			{
				m_Device = other.m_Device;
				m_Adapter = other.m_Adapter;

				other.m_Device = nullptr;
				other.m_Adapter = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		VKSwapchain* VKDevice::CreateVKSwapchain(VkSurfaceKHR surface)
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		VkDevice VKDevice::GetVkDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void VKDevice::Create(VkInstance instance, const DeviceInfo& deviceInfo)
		{
			uint32 adapterCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(instance, &adapterCount, nullptr);
			if (result != VK_SUCCESS)
				return;


			std::vector<VkPhysicalDevice> adapters;
			adapters.resize(adapterCount);

			result = vkEnumeratePhysicalDevices(instance, &adapterCount, adapters.data());
			if (result != VK_SUCCESS)
				return;


			m_Adapter = adapters[deviceInfo.pAdapter->ApiID];

			uint32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_Adapter, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies;
			queueFamilies.resize(queueFamilyCount);

			int32 index = -1;
			vkGetPhysicalDeviceQueueFamilyProperties(m_Adapter, &queueFamilyCount, queueFamilies.data());
			for (uint32 i = 0; i < queueFamilyCount; i++)
			{
				if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
					queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					index = i;
					break;
				}
			}

			if (index < 0)
				return;


			float priority = 1.0f;
			VkDeviceQueueCreateInfo qInfo = {};
			qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			qInfo.pNext = nullptr;
			qInfo.queueCount = queueFamilies[index].queueCount;
			qInfo.pQueuePriorities = &priority;
			qInfo.queueFamilyIndex = index;

			VkPhysicalDeviceFeatures features = {};
			vkGetPhysicalDeviceFeatures(m_Adapter, &features);

			const char* deviceExtensions[] =
			{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			};

			VkDeviceCreateInfo dInfo = {};
			dInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			dInfo.pNext = nullptr;
			dInfo.pEnabledFeatures = &features;
			dInfo.enabledExtensionCount = 1;
			dInfo.ppEnabledExtensionNames = deviceExtensions;
			dInfo.enabledLayerCount = 0;
			dInfo.ppEnabledLayerNames = nullptr;
			dInfo.queueCreateInfoCount = 1;
			dInfo.pQueueCreateInfos = &qInfo;

			result = vkCreateDevice(m_Adapter, &dInfo, nullptr, &m_Device);
			return;
		}
	}
}