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

#include "..\..\Include\DX12\DX12CommandList.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"

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
			D3DRelease_S(m_Allocator);
			D3DRelease_S(m_List);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D12CommandList* DX12CommandList::GetD3D12CommandList() const
		{
			return m_List;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D12CommandAllocator* DX12CommandList::GetD3D12CommandAllocator() const
		{
			return m_Allocator;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D12GraphicsCommandList* DX12CommandList::GetD3D12GraphicsCommandList() const
		{
			return m_List;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12CommandList::Reset() const
		{
			if (FAILED(m_Allocator->Reset()))
				return false;

			if (FAILED(m_List->Reset(m_Allocator, nullptr)))
				return false;

			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12CommandList::SetName(const std::string& name)
		{
			D3D12SetName(m_List, name);
			D3D12SetName(m_Allocator, name + " : Allocator");
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12CommandList::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12CommandList::GetReferenceCount() const
		{
			return m_References;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12CommandList::AddRef()
		{
			m_References++;
			return m_References;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12CommandList::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12CommandList::Create(ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type, int32 nodeMask)
		{
			using namespace System;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_Allocator));

			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create CommandAllocator. " + DXErrorString(hr));
				return;
			}
			else
			{
				D3D12SetName(m_Allocator, "CommandList : Allocator");
			}



			hr = pD3D12Device->CreateCommandList(nodeMask, type, m_Allocator, pInitalState, IID_PPV_ARGS(&m_List));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create CommandList. " + DXErrorString(hr));
			}
			else
			{
				D3D12SetName(m_List, "CommandList : List");
			}
		}
	}
}

#endif