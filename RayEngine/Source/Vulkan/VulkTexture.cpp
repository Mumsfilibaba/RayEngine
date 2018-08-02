#include "..\..\Include\Vulkan\VulkTexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		VulkTexture::VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
		{
		}
		VulkTexture::VulkTexture(IDevice* pDevice, VkImage image)
		{
		}
		VulkTexture::VulkTexture(VulkTexture && other)
		{
		}
		VulkTexture::~VulkTexture()
		{
		}
		VulkTexture & VulkTexture::operator=(VulkTexture && other)
		{
			// TODO: insert return statement here
		}
		VkImage VulkTexture::GetVkImage() const
		{
			return VkImage();
		}
		RESOURCE_STATE VulkTexture::GetResourceState() const
		{
			return RESOURCE_STATE();
		}
		IDevice * VulkTexture::GetDevice() const
		{
			return nullptr;
		}
		IReferenceCounter * VulkTexture::QueryReference()
		{
			return nullptr;
		}
		uint32 VulkTexture::GetReferenceCount() const
		{
			return uint32();
		}
		void VulkTexture::Release() const
		{
		}
		uint32 VulkTexture::AddRef()
		{
			return uint32();
		}
		void VulkTexture::Create(IDevice * pDevice, const ResourceData * const pInitialData, const TextureInfo & info)
		{
		}
	}
}