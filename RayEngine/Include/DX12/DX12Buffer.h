#pragma once

#include "..\..\Include\Graphics\IBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Buffer final : public IBuffer, public DX12Resource
		{
		public:
			DX12Buffer(const DX12Buffer& other) = delete;
			DX12Buffer& operator=(const DX12Buffer& other) = delete;
			DX12Buffer(DX12Buffer&& other) = delete;
			DX12Buffer& operator=(DX12Buffer&& other) = delete;

		public:
			DX12Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			~DX12Buffer();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;
			D3D12_VERTEX_BUFFER_VIEW GetD3D12VertexBufferView() const;
			D3D12_INDEX_BUFFER_VIEW GetD3D12IndexBufferView() const;

			void* Map(int32 subresource) override final;
			void Unmap() override final;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ResourceData* pInitalData, const BufferInfo& info);
			void CreateView(const BufferInfo& usage);

		private:
			DX12Device* m_Device;
			BUFFER_USAGE m_BufferType;
			union 
			{
				D3D12_CPU_DESCRIPTOR_HANDLE Constant;
				D3D12_VERTEX_BUFFER_VIEW Vertex;
				D3D12_INDEX_BUFFER_VIEW Index;
			} m_Views;
		};
	}
}

#endif