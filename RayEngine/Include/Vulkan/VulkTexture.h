#pragma once

#include "..\Graphics\ITexture.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkTexture : public ITexture
		{
		public:
			VulkTexture(const VulkTexture& other) = delete;
			VulkTexture& operator=(const VulkTexture& other) = delete;
			VulkTexture(VulkTexture&& other) = delete;
			VulkTexture& operator=(VulkTexture&& other) = delete;

		public:
			VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			VulkTexture(IDevice* pDevice, VkImage image);
			~VulkTexture();

			void InvalidateResource();
			VkImage GetVkImage() const;

			RESOURCE_STATE GetResourceState() const override final;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			IDevice* m_Device;
			VkImage m_Image;
		};
	}
}