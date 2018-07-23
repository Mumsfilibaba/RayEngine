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
			D3D12_RESOURCE_DESC desc = DX12Resource::CreateDescBuffer(info.Count, info.Stride, D3D12_RESOURCE_FLAG_NONE);
			m_Buffer = DX12Resource(device, info.Name, nullptr, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);
		}
	}
}