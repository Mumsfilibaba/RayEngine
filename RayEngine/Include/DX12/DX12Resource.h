#pragma once

#include "DX12Common.h"
#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Resource
		{
		public:
			DX12Resource(const DX12Resource& other) = delete;
			DX12Resource& operator=(const DX12Resource& other) = delete;
			DX12Resource(DX12Resource&& other) = delete;
			DX12Resource& operator=(DX12Resource&& other) = delete;

		protected:
			DX12Resource();
			~DX12Resource();

		public:
			ID3D12Resource* GetD3D12Resource() const;
			D3D12_RESOURCE_STATES GetD3D12State() const;
			void SetD3D12State(D3D12_RESOURCE_STATES state) const;

		protected:
			ID3D12Resource* m_Resource;
			mutable D3D12_RESOURCE_STATES m_State;
		};
	}
}

#endif