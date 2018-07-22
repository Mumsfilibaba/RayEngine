#pragma once

#include "..\..\Include\Graphics\IBuffer.h"
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Buffer : public IBuffer
		{
		public:
			DX12PipelineState(const DX12PipelineState& other) = delete;
			DX12PipelineState& operator=(const DX12PipelineState& other) = delete;

		public:
			DX12Buffer(ID3D12Device* device, const BufferInfo& info);
			~DX12Buffer();

		private:
			void Create(ID3D12Device* device, const BufferInfo& info);

		private:
			DX12Resource m_Buffer;
		};
	}
}