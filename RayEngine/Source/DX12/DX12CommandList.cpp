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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12CommandList.h>
#include <DX12/DX12Device.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12CommandList::DX12CommandList(DX12Device* pDevice, ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type, int32 nodeMask)
			: m_List(nullptr),
			m_Allocator(nullptr),
			m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pInitalState, type, nodeMask);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12CommandList::~DX12CommandList()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12CommandList::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12CommandList::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12CommandList::Create(ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type, int32 nodeMask)
		{
			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();

			HRESULT hr = pD3D12Device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_Allocator));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create CommandAllocator. " + DXErrorString(hr));
				return;
			}

			hr = pD3D12Device->CreateCommandList(nodeMask, type, m_Allocator.Get(), pInitalState, IID_PPV_ARGS(&m_List));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create CommandList. " + DXErrorString(hr));
			}
			else
			{
				Close();
			}
		}
	}
}

#endif