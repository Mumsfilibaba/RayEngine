#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IRenderTargetView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12View.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12RenderTargetView : public IRenderTargetView, public DX12View
		{
		public:
			DX12RenderTargetView(const DX12RenderTargetView& other) = delete;
			DX12RenderTargetView& operator=(const DX12RenderTargetView& other) = delete;
			DX12RenderTargetView(DX12RenderTargetView&& other) = delete;
			DX12RenderTargetView& operator=(DX12RenderTargetView&& other) = delete;

		public:
			DX12RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info);
			~DX12RenderTargetView();

			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const RenderTargetViewInfo& info);

		private:
			IDevice* m_Device;
		};
	}
}

#endif