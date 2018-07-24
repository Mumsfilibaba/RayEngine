#pragma once

#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Resource
		{
		public:
			DX12Resource(const DX12Resource& other) = delete;
			DX12Resource& operator=(const DX12Resource& other) = delete;
		
		public:
			DX12Resource();
			DX12Resource(ID3D12Device* pDevice, const std::string& name, D3D12_CLEAR_VALUE* pClearValue,
				const D3D12_RESOURCE_DESC& desc,  D3D12_RESOURCE_STATES initalState, RESOURCE_USAGE usage,
				CPU_ACCESS_FLAG cpuAccess);
			DX12Resource(ID3D12Resource* pResource);
			DX12Resource(DX12Resource&& other);
			~DX12Resource();

			void* Map(int32 subresource);
			void Unmap();

			void GetDesc(D3D12_RESOURCE_DESC& desc);

			ID3D12Resource* GetD3D12Resource() const;
			D3D12_RESOURCE_STATES GetD3D12State() const;
			void SetD3D12State(D3D12_RESOURCE_STATES state);

			DX12Resource& operator=(DX12Resource&& other);

		private:
			ID3D12Resource* m_Resource;
			mutable D3D12_RESOURCE_STATES m_State;

		protected:
			void Create(ID3D12Device* pDevice, const std::string& name, D3D12_CLEAR_VALUE* pClearValue, const D3D12_RESOURCE_DESC& desc,
				D3D12_RESOURCE_STATES initalState, RESOURCE_USAGE usage, CPU_ACCESS_FLAG cpuAccess);
		};
	}
}