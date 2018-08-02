#include "..\..\Include\DX12\DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource()
			: m_Resource(nullptr),
			m_State(D3D12_RESOURCE_STATE_COMMON)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::~DX12Resource()
		{
			D3DRelease_S(m_Resource);
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Resource::GetD3D12Resource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES DX12Resource::GetD3D12State() const
		{
			return m_State;
		}



		/////////////////////////////////////////////////////////////
		void DX12Resource::SetD3D12State(D3D12_RESOURCE_STATES state) const
		{
			m_State = state;
		}
	}
}