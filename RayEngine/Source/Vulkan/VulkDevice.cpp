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
#include "..\..\Include\Vulkan\VulkRootLayout.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkDevice::VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo)
			: mFactory(nullptr),
			m_Device(nullptr),
			m_Adapter(nullptr),
			mReferences(0)
		{
			AddRef();
			mFactory = pFactory->QueryReference<VulkFactory>();

			Create(deviceInfo);
		}



		/////////////////////////////////////////////////////////////
		VulkDevice::~VulkDevice()
		{
			ReRelease_S(mFactory);

			if (m_Device != nullptr)
			{
				vkDestroyDevice(m_Device, nullptr);
				m_Device = nullptr;
			}
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
		bool VulkDevice::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewInfo& info)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreateSampler(ISampler** ppSampler, const SamplerInfo& info)
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
		bool VulkDevice::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutInfo& info)
		{
			return ((*ppRootLayout= new VulkRootLayout(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool VulkDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return ((*ppPipelineState = new VulkPipelineState(this, info)));
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::SetName(const std::string& name)
		{
			//Not relevant for now
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = mFactory->QueryReference<VulkFactory>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDevice::GetReferenceCount() const
		{
			return mReferences;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDevice::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDevice::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		System::Log* VulkDevice::GetDeviceLog()
		{
			return &mLog;
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

		bool VulkDevice::GetImmediateContext(IDeviceContext ** ppContext)
		{
			return false;
		}

		bool VulkDevice::CreateDefferedContext(IDeviceContext ** ppContext)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void VulkDevice::Create(const DeviceInfo& deviceInfo)
		{
			using namespace System;

			VkInstance instance = mFactory->GetVkInstance();

			uint32 adapterCount = 0;
			VkResult result = vkEnumeratePhysicalDevices(instance, &adapterCount, nullptr);
			if (result != VK_SUCCESS)
			{
				mLog.Write(LOG_SEVERITY_ERROR, "Vulkan: Failed to enumerate Adapter.");
				return;
			}

			std::vector<VkPhysicalDevice> adapters;
			adapters.resize(adapterCount);
			result = vkEnumeratePhysicalDevices(instance, &adapterCount, adapters.data());
			if (result != VK_SUCCESS)
			{
				mLog.Write(LOG_SEVERITY_ERROR, "Vulkan: Failed to enumerate Adapter.");
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
				mLog.Write(LOG_SEVERITY_ERROR, "Vulkan: No supported queuefamilies.");
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
				mLog.Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create device.");
				return;
			}
		}
	}
}