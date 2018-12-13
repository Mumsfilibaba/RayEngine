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
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class DX12Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12DynamicUploadHeap final : public DX12Resource, public IObject
		{
			RE_IMPLEMENT_INTERFACE(DX12DynamicUploadHeap);

		public:
			DX12DynamicUploadHeap(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes);
			~DX12DynamicUploadHeap();

			inline ID3D12Heap* GetD3D12Heap() const
			{
				return m_Heap.Get();
			}

			void SetData(const void* pData, int32 size);
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(uint32 alignment, uint32 sizeInBytes);

		private:
			DX12Device* m_Device;
			Microsoft::WRL::ComPtr<ID3D12Heap> m_Heap;

			int32 m_SizeInBytes;

			CounterType m_References;
		};
	}
}

#endif