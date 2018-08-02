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

			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const RenderTargetViewInfo& info);

		private:
			mutable uint32 m_ReferenceCount;
		};
	}
}