#pragma once

#include "..\..\Include\Graphics\IDevice.h"
#include "..\..\Include\Graphics\IDepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12View.h"
#include "DX12Device.h"

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

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const DepthStencilViewInfo& info);

		private:
			DX12Device* m_Device;
		};
	}
}

#endif