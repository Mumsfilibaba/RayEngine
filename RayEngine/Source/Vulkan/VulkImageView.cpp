#include "..\..\Include\Vulkan\VulkImageView.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkImageView::VulkImageView(IDevice* pDevice)
			: m_Device(nullptr),
			m_View(nullptr)
		{
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		VulkImageView::~VulkImageView()
		{
			if (m_View != nullptr)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyImageView(vkDevice, m_View, nullptr);
				m_View = nullptr;
			}


			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		VkImageView VulkImageView::GetVkImageView() const
		{
			return m_View;
		}
	}
}