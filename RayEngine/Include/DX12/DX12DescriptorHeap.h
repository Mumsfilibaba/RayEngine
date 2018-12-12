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
#include "Graphics\IDeviceObject.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12DescriptorHeap final : public IDeviceObject
		{
			RE_IMPLEMENT_INTERFACE(DX12DescriptorHeap);

		public:
			DX12DescriptorHeap(IDevice* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, int32 num);
			~DX12DescriptorHeap();

			inline int32 GetDescriptorsLeft() const
			{
				return m_Count - m_UsedCount;
			}

			DX12DescriptorHandle GetNext() const;

			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, int32 num);

		private:
			DX12Device* m_Device;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_Heap;
			
			int32 m_DescriptorSize;
			int32 m_Count;
			mutable int32 m_UsedCount;

			IObject::CounterType m_References;
		};
	}
}

#endif