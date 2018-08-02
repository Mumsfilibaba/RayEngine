#pragma once

#include "..\..\Include\Graphics\IDevice.h"
#include "..\..\Include\Graphics\IDepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12View.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DepthStencilView : public IDepthStencilView, public DX12View
		{
		public:
			DX12DepthStencilView(const DX12DepthStencilView& other) = delete;
			DX12DepthStencilView& operator=(const DX12DepthStencilView& other) = delete;
			DX12DepthStencilView(DX12DepthStencilView&& other) = delete;
			DX12DepthStencilView& operator=(DX12DepthStencilView&& other) = delete;

		public:
			DX12DepthStencilView(IDevice* pDevice, const DepthStencilViewInfo& info);
			~DX12DepthStencilView();

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
			
			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif