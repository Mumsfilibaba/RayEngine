#pragma once

#include "..\..\Include\Graphics\IDevice.h"
#include "..\..\Include\Graphics\IDepthStencilView.h"
#include "DX12ViewBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DepthStencilView : public IDepthStencilView, public DX12ViewBase
		{
		public:
			DX12DepthStencilView(const DX12DepthStencilView& other) = delete;
			DX12DepthStencilView& operator=(const DX12DepthStencilView& other) = delete;

		public:
			DX12DepthStencilView(const IDevice* pDevice, const DepthStencilViewInfo& info);
			DX12DepthStencilView(DX12DepthStencilView&& other);
			~DX12DepthStencilView();

			DX12DepthStencilView& operator=(DX12DepthStencilView&& other);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(const IDevice* pDevice, const DepthStencilViewInfo& info);

		private:
			mutable uint32 m_ReferenceCount;
		};
	}
}