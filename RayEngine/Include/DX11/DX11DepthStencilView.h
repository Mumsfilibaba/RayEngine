#pragma once

#include "..\Graphics\IDepthStencilView.h"
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11DepthStencilView : public IDepthStencilView
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
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const DepthStencilViewInfo& info);

		private:
			IDevice* m_Device;
			ID3D11DepthStencilView* m_View;
		};
	}

}