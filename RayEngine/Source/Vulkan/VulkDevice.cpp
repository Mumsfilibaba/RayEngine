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
#include "..\..\Include\Vulkan\VulkDevice.h"
#include "..\..\Include\Vulkan\VulkFactory.h"
#include "..\..\Include\Vulkan\VulkTexture.h"
#include "..\..\Include\Vulkan\VulkRenderTargetView.h"
#include "..\..\Include\Vulkan\VulkShader.h"
#include "..\..\Include\Vulkan\VulkPipelineState.h"
#include "..\..\Include\Vulkan\VulkRootSignature.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkDevice::VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo)
			: m_Factory(nullptr),
			m_Device(nullptr),
			m_Adapter(nullptr)
		{
			AddRef();
			m_Factory = reinterpret_cast<VulkFactory*>(pFactory->QueryReference());

			Create(deviceInfo);
		}



		/////////////////////////////////////////////////////////////
		VulkDevice::~VulkDevice()
		{
			ReRelease_S(m_Factory);

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
		bool VulkDevice::CreateShader(IShader** ppShader, const ShaderInfo& info)
		{
			return ((*ppShader = new VulkShader(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return ((*ppView = new VulkRenderTargetView(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return ((*ppTexture = new VulkTexture(this, pInitialData, info)));
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info)
		{
			return ((*ppRootSignature = new VulkRootSignature(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return ((*ppPipelineState = new VulkPipelineState(this, info)));
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<VulkFactory*>(m_Factory->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		System::Log* VulkDevice::GetDeviceLog()
		{
			return &m_Log;
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
		void VulkDevice::Create(const DeviceInfo& deviceInfo)
		{
			using namespace System;

			VkInstance instance = m_Factory->GetVkInstance();

			uint32 adapterCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(instance, &adapterCount, nullptr);
			if (result != VK_SUCCESS)
			{
				m_Log.Write(LOG_SEVERITY_ERROR, "Vulkan: Failed to enumerate Adapter.");
				return;
			}

			std::vector<VkPhysicalDevice> adapters;
			adapters.resize(adapterCount);
			result = vkEnumeratePhysicalDevices(instance, &adapterCount, adapters.data());
			if (result != VK_SUCCESS)
			{
				m_Log.Write(LOG_SEVERITY_ERROR, "Vulkan: Failed to enumerate Adapter.");
				return;
			}
			else
			{
				m_Adapter = adapters[deviceInfo.pAdapter->ApiID];
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
				m_Log.Write(LOG_SEVERITY_ERROR, "Vulkan: No supported queuefamilies.");
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
				m_Log.Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create device.");
				return;
			}
		}
	}
}