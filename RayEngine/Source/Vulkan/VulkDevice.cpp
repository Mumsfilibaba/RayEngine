#include <vector>
#include "..\..\Include\Vulkan\VulkDevice.h"
#include "..\..\Include\Vulkan\VulkFactory.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkDevice::VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo)
			: m_Factory(nullptr),
			m_Device(nullptr),
			m_Adapter(nullptr),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory->QueryReference());
			Create(pFactory, deviceInfo);
		}



		/////////////////////////////////////////////////////////////
		VulkDevice::VulkDevice(VulkDevice&& other)
			: m_Factory(other.m_Factory),
			m_Device(other.m_Device),
			m_Adapter(other.m_Adapter),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Factory = nullptr;
			other.m_Device = nullptr;
			other.m_Adapter = nullptr;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		VulkDevice::~VulkDevice()
		{
			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}

			if (m_Device != nullptr)
			{
				vkDestroyDevice(m_Device, nullptr);
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateShader(IShader** ppShader, const ShaderByteCode& byteCode)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		System::Log* VulkDevice::GetDeviceLog()
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		VulkDevice& VulkDevice::operator=(VulkDevice&& other)
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
		IFactory* VulkDevice::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		VkDevice VulkDevice::GetVkDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		VkPhysicalDevice VulkDevice::GetVkPhysicalDevice() const
		{
			return m_Adapter;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* VulkDevice::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkDevice::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkDevice::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::Create(IFactory* pFactory, const DeviceInfo& deviceInfo)
		{
			VkInstance instance = reinterpret_cast<VulkFactory*>(pFactory)->GetVkInstance();

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