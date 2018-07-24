#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Buffer.h"


namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
			: m_Buffer(),
			m_UploadBuffer()
		{
			Create(pDevice, pInitalData, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Create(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
		{
			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			const DX12CommandQueue* pQueue = reinterpret_cast<const DX12Device*>(pDevice)->GetDX12CommandQueue();


			D3D12_RESOURCE_DESC desc = DX12Resource::CreateDescBuffer(info.Count, info.Stride, D3D12_RESOURCE_FLAG_NONE);
			m_Buffer = DX12Resource(pD3D12Device, info.Name, nullptr, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);

			if (pInitalData != nullptr)
			{
				//TODO: Should be changed later to take different CPU_ACCESS into account??

				m_UploadBuffer = DX12Resource(pD3D12Device, (info.Name + ": UploadBuffer"), nullptr, desc,
					D3D12_RESOURCE_STATE_GENERIC_READ, RESOURCE_USAGE_DYNAMIC, CPU_ACCESS_FLAG_WRITE);


				void* gpuData = m_UploadBuffer.Map(0);
				memcpy(gpuData, pInitalData->pData, pInitalData->ByteStride * pInitalData->WidthOrCount);
				m_UploadBuffer.Unmap();


				pQueue->TransitionResource(m_Buffer, m_Buffer.GetState(), D3D12_RESOURCE_STATE_COPY_DEST, 0);
			}
		}
	}
}