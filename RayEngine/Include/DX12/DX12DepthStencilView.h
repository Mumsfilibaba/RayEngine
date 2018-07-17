#pragma once

#include "..\..\Include\Graphics\IDepthStencilView.h"
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
			DX12DepthStencilView(ID3D12Device* device, DX12DescriptorHeap& heap, const DepthStencilViewInfo& info);
			DX12DepthStencilView(DX12DepthStencilView&& other);
			~DX12DepthStencilView();

			DX12DepthStencilView& operator=(DX12DepthStencilView&& other);

		private:
			void Create(ID3D12Device* device, DX12DescriptorHeap& heap, const DepthStencilViewInfo& info);

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE m_View;
		};
	}
}