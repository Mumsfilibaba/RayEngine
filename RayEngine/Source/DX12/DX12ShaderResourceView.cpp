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

#include "..\..\Include\DX12\DX12ShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12ShaderResourceView::DX12ShaderResourceView(IDevice* pDevice, const ShaderResourceViewDesc* pDesc)
			: DX12View(),
			m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12ShaderResourceView::~DX12ShaderResourceView()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12ShaderResourceView::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12ShaderResourceView::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12ShaderResourceView::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::Create(const ShaderResourceViewDesc* pDesc)
		{
			m_Resource = pDesc->pResource->QueryReference<DX12Resource>();
			ID3D12Resource* pD3D12Resource = m_Resource->GetD3D12Resource();

			const DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12ResourceHeap();
			m_View = pDX12Heap->GetNext(m_Resource);


			D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
			if (pDesc->ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				
				if (pDesc->Flags & SHADER_RESOURCE_VIEW_FLAG_RAW_BUFFER)
					desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
				else
					desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

				desc.Buffer.FirstElement = pDesc->Buffer.StartElement;
				desc.Buffer.NumElements = pDesc->Buffer.ElementCount;
				desc.Buffer.StructureByteStride = pDesc->Buffer.ElementStrideBytes;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipLevels = pDesc->Texture1D.MipLevels;
				desc.Texture1D.MostDetailedMip = pDesc->Texture1D.MostDetailedMip;
				desc.Texture1D.ResourceMinLODClamp = pDesc->Texture1D.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipLevels = pDesc->Texture1DArray.MipLevels;
				desc.Texture1DArray.MostDetailedMip = pDesc->Texture1DArray.MostDetailedMip;
				desc.Texture1DArray.ArraySize = pDesc->Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = pDesc->Texture1DArray.FirstArraySlice;
				desc.Texture1DArray.ResourceMinLODClamp = pDesc->Texture1DArray.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = pDesc->Texture2D.MipLevels;
				desc.Texture2D.MostDetailedMip = pDesc->Texture2D.MostDetailedMip;
				desc.Texture2D.PlaneSlice = pDesc->Texture2D.PlaneSlice;
				desc.Texture2D.ResourceMinLODClamp = pDesc->Texture2D.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipLevels = pDesc->Texture2DArray.MipLevels;
				desc.Texture2DArray.MostDetailedMip = pDesc->Texture2DArray.MostDetailedMip;
				desc.Texture2DArray.ArraySize = pDesc->Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = pDesc->Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.PlaneSlice = pDesc->Texture2DArray.PlaneSlice;
				desc.Texture2DArray.ResourceMinLODClamp = pDesc->Texture2DArray.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = pDesc->Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = pDesc->Texture2DMSArray.FirstArraySlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = pDesc->Texture3D.MipLevels;
				desc.Texture3D.MostDetailedMip = pDesc->Texture3D.MostDetailedMip;
				desc.Texture3D.ResourceMinLODClamp = pDesc->Texture3D.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels = pDesc->TextureCube.MipLevels;
				desc.TextureCube.MostDetailedMip = pDesc->TextureCube.MostDetailedMip;
				desc.TextureCube.ResourceMinLODClamp = pDesc->TextureCube.MinLODClamp;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = pDesc->TextureCube.MipLevels;
				desc.TextureCubeArray.MostDetailedMip = pDesc->TextureCubeArray.MostDetailedMip;
				desc.TextureCubeArray.First2DArrayFace = pDesc->TextureCubeArray.First2DArrayFace;
				desc.TextureCubeArray.NumCubes = pDesc->TextureCubeArray.CubeCount;
				desc.TextureCubeArray.ResourceMinLODClamp = pDesc->TextureCubeArray.MinLODClamp;
			}


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateShaderResourceView(pD3D12Resource, nullptr, m_View.CpuDescriptor);
		}
	}
}

#endif