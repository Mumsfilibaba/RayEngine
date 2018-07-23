#pragma once

#include "..\Graphics\IRenderTargetView.h"
#include "DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12RenderTargetView : public IRenderTargetView
		{
		public:
			DX12RenderTargetView(const DX12RenderTargetView& other) = delete;
			DX12RenderTargetView& operator=(const DX12RenderTargetView& other) = delete;

		public:
			DX12RenderTargetView(ID3D12Device* device, DX12DescriptorHeap& heap, const RenderTargetViewInfo& info);
			DX12RenderTargetView(DX12RenderTargetView&& other);
			~DX12RenderTargetView();

			DX12RenderTargetView& operator=(DX12RenderTargetView&& other);

		private:
			void Create(ID3D12Device* device, DX12DescriptorHeap& heap, const RenderTargetViewInfo& info);

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE m_View;
		};
	}
}