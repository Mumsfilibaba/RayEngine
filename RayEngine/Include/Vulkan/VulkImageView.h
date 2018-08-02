#pragma once

#include "..\Graphics\IDevice.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkImageView
		{
		public:
			VulkImageView(const VulkImageView& other) = delete;
			VulkImageView& operator=(const VulkImageView& other) = delete;
			VulkImageView(VulkImageView&& other) = delete;
			VulkImageView& operator=(VulkImageView&& other) = delete;

		protected:
			VulkImageView(IDevice* pDevice);
			~VulkImageView();

		public:
			VkImageView GetVkImageView() const;

		protected:
			IDevice* m_Device;
			VkImageView m_View;
		};
	}
}