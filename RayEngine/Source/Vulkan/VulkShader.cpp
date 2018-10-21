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

#include "..\..\Include\Vulkan\VulkShader.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkShader::VulkShader(IDevice* pDevice, const ShaderDesc& info)
			: m_Device(nullptr),
			m_Module(VK_NULL_HANDLE),
			m_Type(SHADER_TYPE_UNKNOWN),
			mReferences(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(info);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkShader::~VulkShader()
		{
			if (m_Module != VK_NULL_HANDLE)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyShaderModule(vkDevice, m_Module, nullptr);
				m_Module = VK_NULL_HANDLE;
			}

			ReRelease_S(m_Device);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		const std::string& VulkShader::GetEntryPoint() const
		{
			return m_EntryPoint;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkShaderModule VulkShader::GetVkShaderModule() const
		{
			return m_Module;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SHADER_TYPE VulkShader::GetType() const
		{
			return m_Type;
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::SetName(const std::string & name)
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::Create(const ShaderDesc& info)
		{
			using namespace System;

			//TODO: Compile Shader here

			VkShaderModuleCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;
			//desc.codeSize = byteCode.GetSize();
			//desc.pCode = reinterpret_cast<const uint32*>(byteCode.GetBytes());

			VkDevice vkDevice = m_Device->GetVkDevice();
			VkResult result = vkCreateShaderModule(vkDevice, &desc, nullptr, &m_Module);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create ShaderModule.");
			}
			else
			{
				m_Type = info.Type;
				m_EntryPoint = info.EntryPoint;
			}
		}
	}
}