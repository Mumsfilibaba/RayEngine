#pragma once

#include "..\..\Include\Graphics\IDevice.h"
#include "..\..\Include\Graphics\IDepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DepthStencilView : public IDepthStencilView
		{
		public:
			DX12DepthStencilView(const DX12DepthStencilView& other) = delete;
			DX12DepthStencilView& operator=(const DX12DepthStencilView& other) = delete;

		public:
			DX12DepthStencilView(IDevice* pDevice, const DepthStencilViewInfo& info);
			DX12DepthStencilView(DX12DepthStencilView&& other);
			~DX12DepthStencilView();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;

			DX12DepthStencilView& operator=(DX12DepthStencilView&& other);

			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const DepthStencilViewInfo& info);

		private:
			IDevice* m_Device;
			
			D3D12_CPU_DESCRIPTOR_HANDLE m_View;
			
			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif