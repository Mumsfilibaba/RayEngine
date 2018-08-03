#pragma once

#include "..\Graphics\IRootSignature.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkRootSignature : public IRootSignature
		{
		public:
			VulkRootSignature(const VulkRootSignature& other) = delete;
			VulkRootSignature& operator=(const VulkRootSignature& other) = delete;
			VulkRootSignature(VulkRootSignature&& other) = delete;
			VulkRootSignature& operator=(VulkRootSignature&& other) = delete;

		public:
			VulkRootSignature(IDevice* pDevice, const RootSignatureInfo& info);
			~VulkRootSignature();

			VkPipelineLayout GetVkPipelineLayout() const;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const RootSignatureInfo& info);

		private:
			IDevice* m_Device;
			VkRenderPass m_RenderPass;
			VkPipelineLayout m_RootSignature;
		};
	}
}