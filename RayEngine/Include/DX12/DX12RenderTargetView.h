#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IRenderTargetView.h"
#include "DX12ViewBase.h"

#if defined(RE_PLATFROM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12RenderTargetView : public IRenderTargetView
		{
		public:
			DX12RenderTargetView(const DX12RenderTargetView& other) = delete;
			DX12RenderTargetView& operator=(const DX12RenderTargetView& other) = delete;

		public:
			DX12RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info);
			DX12RenderTargetView(DX12RenderTargetView&& other);
			~DX12RenderTargetView();

			DX12RenderTargetView& operator=(DX12RenderTargetView&& other);

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;

			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const RenderTargetViewInfo& info);

		private:
			IDevice* m_Device;

			D3D12_CPU_DESCRIPTOR_HANDLE m_View;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif