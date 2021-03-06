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

#include <RayEngine.h>
#include <Vulkan/VulkDevice.h>
#include <Vulkan/VulkShader.h>
#include <Vulkan/VulkFactory.h>
#include <Vulkan/VulkTexture.h>
#include <Vulkan/VulkRootLayout.h>
#include <Vulkan/VulkPipelineState.h>
#include <Vulkan/VulkRenderTargetView.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkDevice::VulkDevice(const DeviceDesc* pDesc)
			: m_Device(nullptr),
			m_Adapter(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkDevice::~VulkDevice()
		{
			if (m_Device != nullptr)
			{
				vkDestroyDevice(m_Device, nullptr);
				m_Device = nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateShader(IShader** ppShader, const ShaderDesc* pDesc)
		{
			return ((*ppShader = new VulkShader(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc)
		{
			return ((*ppView = new VulkRenderTargetView(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			return ((*ppTexture = new VulkTexture(this, pInitialData, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc)
		{
			return ((*ppRootLayout= new VulkRootLayout(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc)
		{
			return ((*ppPipelineState = new VulkPipelineState(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkDevice::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}

		void VulkDevice::GetAdapterDesc(AdapterDesc * pDesc) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkDevice::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkDevice::Release()
		{
			CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}


		IRenderer* VulkDevice::CreateRenderer()
		{
			return nullptr;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::GetImmediateContext(IDeviceContext** ppContext)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkDevice::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkDevice::Create(const DeviceDesc* pDesc)
		{
			VkInstance instance = 0;// m_Factory->GetVkInstance();

			uint32 adapterCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(instance, &adapterCount, nullptr);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Failed to enumerate Adapter.");
				return;
			}

			std::vector<VkPhysicalDevice> adapters;
			adapters.resize(adapterCount);
			result = vkEnumeratePhysicalDevices(instance, &adapterCount, adapters.data());
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Failed to enumerate Adapter.");
				return;
			}
			else
			{
				m_Adapter = adapters[0];
			}


			uint32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_Adapter, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies;
			queueFamilies.resize(queueFamilyCount);

			//TODO: Check for presentation support

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
			{
				LOG_ERROR("Vulkan: No supported queuefamilies.");
				return;
			}


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
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create device.");
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}