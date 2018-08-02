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

		public:
			VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			VulkTexture(IDevice* pDevice, VkImage image);
			VulkTexture(VulkTexture&& other);
			~VulkTexture();

			VulkTexture& operator=(VulkTexture&& other);

			VkImage GetVkImage() const;

			RESOURCE_STATE GetResourceState() const override final;
			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			IDevice* m_Device;

			VkImage m_Image;

			mutable uint32 m_ReferenceCount;
		};
	}
}