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

#include "../../Include/Debug/Debug.h"
#include "../../Include/Vulkan/VulkShader.h"
#include "../../Include/Vulkan/VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkShader::VulkShader(IDevice* pDevice, const ShaderDesc* pDesc)
			: m_Device(nullptr),
			m_Module(VK_NULL_HANDLE),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pDesc);
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


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::SetName(const std::string& name)
		{
			//Not relevant for now
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::GetDesc(ShaderDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkShader::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkShader::Create(const ShaderDesc* pDesc)
		{
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
				LOG_ERROR("Vulkan: Could not create ShaderModule.");
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}