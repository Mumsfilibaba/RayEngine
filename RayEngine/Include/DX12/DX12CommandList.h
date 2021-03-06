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

#pragma once
#include <Interfaces/IObject.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12CommandList final : public IObject
		{
			RE_IMPLEMENT_INTERFACE(DX12CommandList);

		public:
			DX12CommandList(DX12Device* pDevice, ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type);
			~DX12CommandList();

			inline bool Reset() const
			{
				HRESULT hr = m_List->Reset(m_Allocator.Get(), nullptr);
				return SUCCEEDED(hr);
			}

			inline bool ResetAllocator() const
			{
				HRESULT hr = m_Allocator->Reset();
				return SUCCEEDED(hr);
			}

			inline bool Close() const
			{
				HRESULT hr = m_List->Close();
				return SUCCEEDED(hr);
			}

			inline ID3D12GraphicsCommandList* GetGraphicsList() const
			{
				return m_List.Get();
			}
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type);

		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_Allocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_List;
			CounterType m_References;
		};
	}
}

#endif