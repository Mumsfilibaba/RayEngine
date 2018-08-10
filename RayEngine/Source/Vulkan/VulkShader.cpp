#include "..\..\Include\Vulkan\VulkShader.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkShader::VulkShader(IDevice* pDevice, const ShaderInfo& info)
			: m_Device(nullptr),
			m_Module(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = QueryVulkDevice(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		VulkShader::~VulkShader()
		{
			if (m_Module != nullptr)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyShaderModule(vkDevice, m_Module, nullptr);
				m_Module = nullptr;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		const std::string& VulkShader::GetEntryPoint() const
		{
			return m_EntryPoint;
		}



		/////////////////////////////////////////////////////////////
		VkShaderModule VulkShader::GetVkShaderModule() const
		{
			return m_Module;
		}



		/////////////////////////////////////////////////////////////
		SHADER_TYPE VulkShader::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		void VulkShader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryVulkDevice(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkShader::Create(const ShaderInfo& info)
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