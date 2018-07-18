#include "..\..\Include\DX12\DX12PipelineState.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(ID3D12Device* device, const PipelineStateInfo& info)
			: m_PipelineState(nullptr)
		{
			Create(device, info);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(DX12PipelineState&& other)
			: m_PipelineState(other.m_PipelineState)
		{
			other.m_PipelineState = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::~DX12PipelineState()
		{
			D3DRelease_S(m_PipelineState);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState& DX12PipelineState::operator=(DX12PipelineState&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_PipelineState);

				m_PipelineState = other.m_PipelineState;
				other.m_PipelineState = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::Create(ID3D12Device* device, const PipelineStateInfo& info)
		{
		}
	}
}