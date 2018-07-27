#pragma once

#include "..\..\Include\Graphics\IBuffer.h"
#include "DX12CommandQueue.h"

#if defined(RE_PLATFORM_WINDOWS)

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
			DX12Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			DX12Buffer(DX12Buffer&& other);
			~DX12Buffer();

			void* Map(int32 subresource) override final;
			void Unmap() override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

			IDevice* GetDevice() const override final;

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;
			D3D12_VERTEX_BUFFER_VIEW GetD3D12VertexBufferView() const;
			D3D12_INDEX_BUFFER_VIEW GetD3D12IndexBufferView() const;

			DX12Buffer& operator=(DX12Buffer&& other);

			ID3D12Resource* GetD3D12Resource() const;
			D3D12_RESOURCE_STATES GetD3D12State() const;
			void SetD3D12State(D3D12_RESOURCE_STATES state);

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			void CreateView(IDevice* pDevice, const BufferInfo& usage);

		private:
			IDevice* m_Device;
			
			ID3D12Resource* m_Resource;
			BUFFER_USAGE m_BufferType;
			union 
			{
				D3D12_CPU_DESCRIPTOR_HANDLE Constant;
				D3D12_VERTEX_BUFFER_VIEW Vertex;
				D3D12_INDEX_BUFFER_VIEW Index;
			} m_Views;
			mutable D3D12_RESOURCE_STATES m_State;

			mutable uint32 m_ReferenceCounter;
		};
	}
}

#endif