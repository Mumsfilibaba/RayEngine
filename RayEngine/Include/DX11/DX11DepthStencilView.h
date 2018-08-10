#pragma once

#include "..\Graphics\IDepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11DepthStencilView final : public IDepthStencilView
		{
		public:
			DX11DepthStencilView(const DX11DepthStencilView& other) = delete;
			DX11DepthStencilView& operator=(const DX11DepthStencilView& other) = delete;
			DX11DepthStencilView(DX11DepthStencilView&& other) = delete;
			DX11DepthStencilView& operator=(DX11DepthStencilView&& other) = delete;

		public:
			DX11DepthStencilView(IDevice* pDevice, const DepthStencilViewInfo& info);
			~DX11DepthStencilView();

			ID3D11DepthStencilView* GetD3D11DepthStencilView() const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const DepthStencilViewInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11DepthStencilView* m_View;
		};
	}
}

#endif