#include "..\..\Include\DX12\DX12View.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12View::DX12View()
			: m_View()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12View::~DX12View()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHandle DX12View::GetDX12DescriptorHandle() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12View::GetD3D12CpuDescriptorHandle() const
		{
			return m_View.CpuDescriptor;
		}



		/////////////////////////////////////////////////////////////
		D3D12_GPU_DESCRIPTOR_HANDLE DX12View::GetD3D12GpuDescriptorHandle() const
		{
			return m_View.GpuDescriptor;
		}
	}
}