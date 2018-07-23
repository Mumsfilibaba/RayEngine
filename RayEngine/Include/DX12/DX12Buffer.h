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
			DX12Buffer(const DX12Buffer& other) = delete;
			DX12Buffer& operator=(const DX12Buffer& other) = delete;

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