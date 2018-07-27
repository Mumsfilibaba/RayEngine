#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IRenderTargetView.h"
#include "DX12ViewBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12RenderTargetView : public IRenderTargetView, public DX12ViewBase
		{
		public:
			DX12RenderTargetView(const DX12RenderTargetView& other) = delete;
			DX12RenderTargetView& operator=(const DX12RenderTargetView& other) = delete;

		public:
			DX12RenderTargetView(const IDevice* pDevice, const RenderTargetViewInfo& info);
			DX12RenderTargetView(DX12RenderTargetView&& other);
			~DX12RenderTargetView();

			DX12RenderTargetView& operator=(DX12RenderTargetView&& other);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(const IDevice* pDevice, const RenderTargetViewInfo& info);

		private:
			mutable uint32 m_ReferenceCount;
		};
	}
}