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
#include "..\Graphics\IResource.h"
#include "DX12Resource.h"
#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12View
		{
			RE_UNIQUE_OBJECT(DX12View);

		protected:
			inline DX12View()
				: m_Resource(nullptr),
				m_View()
			{
			}

			inline ~DX12View()
			{
				IResource* pResource = reinterpret_cast<IResource*>(m_Resource);
				ReRelease_S(pResource);
			}

		public:
			inline DX12Resource* GetDX12Resource() const
			{
				return m_Resource;
			}

			inline DX12DescriptorHandle GetDX12DescriptorHandleSRVCBVUAV() const
			{
				return m_View;
			}

			inline D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const
			{
				return m_View.CpuDescriptor;
			}

			inline D3D12_GPU_DESCRIPTOR_HANDLE GetD3D12GpuDescriptorHandle() const
			{
				return m_View.GpuDescriptor;
			}

		protected:
			DX12Resource* m_Resource;

			DX12DescriptorHandle m_View;
		};
	}
}

#endif