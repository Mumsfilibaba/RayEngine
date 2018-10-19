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
#include "DX12DescriptorHandle.h"
#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12View
		{
		public:
			DX12View(const DX12View& other) = delete;
			DX12View& operator=(const DX12View& other) = delete;
			DX12View(DX12View&& other) = delete;
			DX12View& operator=(DX12View&& other) = delete;

		protected:
			DX12View();
			~DX12View();

		public:
			DX12Resource* GetD3D12Resource() const;

			DX12DescriptorHandle GetDX12DescriptorHandle() const;

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;

			D3D12_GPU_DESCRIPTOR_HANDLE GetD3D12GpuDescriptorHandle() const;

		protected:
			DX12Resource* m_Resource;

			DX12DescriptorHandle m_View;
		};
	}
}

#endif