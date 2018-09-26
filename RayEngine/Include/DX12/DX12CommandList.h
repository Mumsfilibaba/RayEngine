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
#include "..\Graphics\IDeviceObject.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;



		/////////////////////////////////////////////////////////////
		class DX12CommandList final : public IDeviceObject
		{
			RE_IMPLEMENT_INTERFACE(DX12CommandList);

		public:
			DX12CommandList(DX12Device* pDevice, ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type, int32 nodeMask);
			~DX12CommandList();

			ID3D12CommandList* GetD3D12CommandList() const;
			
			ID3D12CommandAllocator* GetD3D12CommandAllocator() const;
			
			ID3D12GraphicsCommandList* GetD3D12GraphicsCommandList() const;

			bool Reset() const;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(ID3D12PipelineState* pInitalState, D3D12_COMMAND_LIST_TYPE type, int32 nodeMask);

		private:
			DX12Device* m_Device;
			ID3D12GraphicsCommandList* m_List;
			ID3D12CommandAllocator* m_Allocator;

			IObject::CounterType m_References;
		};
	}
}

#endif