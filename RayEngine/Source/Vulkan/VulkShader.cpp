#include "..\..\Include\Vulkan\VulkShader.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkShader::VulkShader(IDevice* pDevice, const ShaderByteCode& byteCode)
			: m_Device(nullptr),
			m_Module(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, byteCode);
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
		IDevice* VulkShader::GetDevice() const
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		void VulkShader::Create(IDevice* pDevice, const ShaderByteCode& byteCode)
		{
			using namespace System;

			VkShaderModuleCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;
			desc.codeSize = byteCode.GetSize();
			desc.pCode = reinterpret_cast<const uint32*>(byteCode.GetBytes());

			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(pDevice)->GetVkDevice();
			VkResult result = vkCreateShaderModule(vkDevice, &desc, nullptr, &m_Module);
			if (result != VK_SUCCESS)
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create ShaderModule");
				return;
			}
			else
			{
				m_Type = byteCode.GetType();
				m_EntryPoint = byteCode.GetEntryPoint();
			}
		}
	}
}