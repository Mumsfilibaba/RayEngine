#pragma once

#include "..\..\Include\Graphics\IBuffer.h"
#include "DX12Resource.h"
#include "DX12CommandQueue.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Buffer : public IBuffer, public DX12Resource
		{
		public:
			DX12Buffer(const DX12Buffer& other) = delete;
			DX12Buffer& operator=(const DX12Buffer& other) = delete;

		public:
			DX12Buffer(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			DX12Buffer(DX12Buffer&& other);
			~DX12Buffer();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;
			D3D12_VERTEX_BUFFER_VIEW GetD3D12VertexBufferView() const;
			D3D12_INDEX_BUFFER_VIEW GetD3D12IndexBufferView() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			void CreateView(const IDevice* pDevice, const BufferInfo& usage);

		private:
			BUFFER_USAGE m_BufferType;
			union 
			{
				D3D12_CPU_DESCRIPTOR_HANDLE Constant;
				D3D12_VERTEX_BUFFER_VIEW Vertex;
				D3D12_INDEX_BUFFER_VIEW Index;
			} m_Views;

			mutable uint32 m_ReferenceCounter;
		};
	}
}