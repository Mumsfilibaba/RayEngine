#include "..\..\Include\DX12\DX12Buffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(ID3D12Device* device, const BufferInfo& info)
			: m_Buffer()
		{
			Create(device, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Create(ID3D12Device* device, const BufferInfo& info)
		{
			m_Buffer = DX12Resource::CreateBuffer(device, info.Count, info.Stride, D3D12_RESOURCE_FLAG_NONE, info.Usage);
		}
	}
}