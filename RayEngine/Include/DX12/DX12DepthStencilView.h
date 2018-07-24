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

		private:
			void Create(const IDevice* pDevice, const DepthStencilViewInfo& info);
		};
	}
}