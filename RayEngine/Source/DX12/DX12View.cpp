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

#include "..\..\Include\Graphics\IResource.h"
#include "..\..\Include\DX12\DX12View.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12View::DX12View()
			: m_Resource(nullptr),
			m_View()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12View::~DX12View()
		{
			IResource* pResource = reinterpret_cast<IResource*>(m_Resource);
			ReRelease_S(pResource);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Resource* DX12View::GetD3D12Resource() const
		{
			return m_Resource;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DescriptorHandle DX12View::GetDX12DescriptorHandle() const
		{
			return m_View;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12View::GetD3D12CpuDescriptorHandle() const
		{
			return m_View.CpuDescriptor;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_GPU_DESCRIPTOR_HANDLE DX12View::GetD3D12GpuDescriptorHandle() const
		{
			return m_View.GpuDescriptor;
		}
	}
}

#endif