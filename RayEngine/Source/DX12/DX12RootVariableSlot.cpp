/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\DX12\DX12RootVariableSlot.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12GraphicsDescriptorRootSlot::DX12GraphicsDescriptorRootSlot(D3D12_RESOURCE_STATES neededState, int32 baseSlot)
			: DX12RootVariableSlot(neededState),
			m_BaseSlot(baseSlot)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12GraphicsDescriptorRootSlot::~DX12GraphicsDescriptorRootSlot()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsDescriptorRootSlot::SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSRVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSRVs[i].GpuDescriptor;
				pCommandList->SetGraphicsRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsDescriptorRootSlot::SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSamplerStates[i].GpuDescriptor;
				pCommandList->SetGraphicsRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsDescriptorRootSlot::SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pUAVs[i].GpuDescriptor;
				pCommandList->SetGraphicsRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsDescriptorRootSlot::SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pBuffers[i].GpuDescriptor;
				pCommandList->SetGraphicsRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		DX12GraphicsRootSignatureSlot::DX12GraphicsRootSignatureSlot(D3D12_RESOURCE_STATES neededState, int32 slot)
			: DX12RootVariableSlot(neededState), 
			m_Slot(slot)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12GraphicsRootSignatureSlot::~DX12GraphicsRootSignatureSlot()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsRootSignatureSlot::SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSRVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pSRVs[i].GpuResourceAdress;
				pCommandList->SetGraphicsRootShaderResourceView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsRootSignatureSlot::SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSamplerStates[i].GpuDescriptor;
				pCommandList->SetGraphicsRootDescriptorTable(m_Slot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsRootSignatureSlot::SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pUAVs[i].GpuResourceAdress;
				pCommandList->SetGraphicsRootUnorderedAccessView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12GraphicsRootSignatureSlot::SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pBuffers[i].GpuResourceAdress;
				pCommandList->SetGraphicsRootConstantBufferView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		DX12ComputeDescriptorRootSlot::DX12ComputeDescriptorRootSlot(D3D12_RESOURCE_STATES neededState, int32 baseSlot)
			: DX12RootVariableSlot(neededState),
			m_BaseSlot(baseSlot)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12ComputeDescriptorRootSlot::~DX12ComputeDescriptorRootSlot()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeDescriptorRootSlot::SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSRVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSRVs[i].GpuDescriptor;
				pCommandList->SetComputeRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeDescriptorRootSlot::SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSamplerStates[i].GpuDescriptor;
				pCommandList->SetComputeRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeDescriptorRootSlot::SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pUAVs[i].GpuDescriptor;
				pCommandList->SetComputeRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeDescriptorRootSlot::SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pBuffers[i].GpuDescriptor;
				pCommandList->SetComputeRootDescriptorTable(m_BaseSlot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		DX12ComputeRootSignatureSlot::DX12ComputeRootSignatureSlot(D3D12_RESOURCE_STATES neededState, int32 slot)
			: DX12RootVariableSlot(neededState),
			m_Slot(slot)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12ComputeRootSignatureSlot::~DX12ComputeRootSignatureSlot()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeRootSignatureSlot::SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSRVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pSRVs[i].GpuResourceAdress;
				pCommandList->SetComputeRootShaderResourceView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeRootSignatureSlot::SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE handle = pSamplerStates[i].GpuDescriptor;
				pCommandList->SetComputeRootDescriptorTable(m_Slot + i, handle);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeRootSignatureSlot::SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pUAVs[i].GpuResourceAdress;
				pCommandList->SetComputeRootUnorderedAccessView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12ComputeRootSignatureSlot::SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_GPU_VIRTUAL_ADDRESS adress = pBuffers[i].GpuResourceAdress;
				pCommandList->SetComputeRootConstantBufferView(m_Slot + i, adress);
			}
		}



		/////////////////////////////////////////////////////////////
		DX12RootVariableSlot::DX12RootVariableSlot(D3D12_RESOURCE_STATES neededState)
			: m_NeededState(neededState)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12RootVariableSlot::~DX12RootVariableSlot()
		{
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES DX12RootVariableSlot::GetNeededD3D12ResourceState() const
		{
			return m_NeededState;
		}
	}
}

#endif