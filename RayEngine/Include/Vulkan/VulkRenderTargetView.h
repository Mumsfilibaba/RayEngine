#pragma once

#include "..\Graphics\IRenderTargetView.h"
#include "VulkImageView.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkRenderTargetView : public IRenderTargetView, public VulkImageView
		{
		public:
			VulkRenderTargetView(VulkRenderTargetView&& other) = delete;
			VulkRenderTargetView& operator=(VulkRenderTargetView&& other) = delete;
			VulkRenderTargetView(const VulkRenderTargetView& other) = delete;
			VulkRenderTargetView& operator=(const VulkRenderTargetView& other) = delete;

		public:
			VulkRenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info);
			~VulkRenderTargetView();

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const RenderTargetViewInfo& info);
		};
	}
}