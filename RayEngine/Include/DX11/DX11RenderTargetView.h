#pragma once

#include "..\Graphics\IRenderTargetView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11RenderTargetView : public IRenderTargetView
		{
		public:
			DX11RenderTargetView(const DX11RenderTargetView& other) = delete;
			DX11RenderTargetView& operator=(const DX11RenderTargetView& other) = delete;
			DX11RenderTargetView(DX11RenderTargetView&& other) = delete;
			DX11RenderTargetView& operator=(DX11RenderTargetView&& other) = delete;

		public:
			DX11RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info);
			~DX11RenderTargetView();

			ID3D11RenderTargetView* GetD3D11RenderTargetView() const;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const RenderTargetViewInfo& info);

		private:
			IDevice* m_Device;
			ID3D11RenderTargetView* m_View;
		};
	}
}

#endif