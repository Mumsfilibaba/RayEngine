#include "..\..\Include\Vulkan\VulkRootSignature.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkRootSignature::VulkRootSignature(IDevice* pDevice, const RootSignatureInfo& info)
			: m_Device(nullptr),
			m_Layout(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		VulkRootSignature::~VulkRootSignature()
		{
			if (m_Layout != nullptr)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyPipelineLayout(vkDevice, m_Layout, nullptr);
				m_Layout = nullptr;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		VkPipelineLayout VulkRootSignature::GetVkPipelineLayout() const
		{
			return m_Layout;
		}

		

		/////////////////////////////////////////////////////////////
		IDevice* VulkRootSignature::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void VulkRootSignature::Create(IDevice* pDevice, const RootSignatureInfo& info)
		{
		}
	}
}