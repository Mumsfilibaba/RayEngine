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
		DX12ShaderResourceView::DX12ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info)
			: DX12View(),
			m_Device(nullptr),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(info);
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
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12ShaderResourceView::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12ShaderResourceView::Release()
		{
			mReferences--;
			IObject::CounterType counter = mReferences;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::Create(const ShaderResourceViewInfo& info)
		{
			m_Resource = info.pResource->QueryReference<DX12Resource>();
			ID3D12Resource* pD3D12Resource = m_Resource->GetD3D12Resource();

			const DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12ResourceHeap();
			m_View = pDX12Heap->GetNext(m_Resource);


			D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
			if (info.ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				
				if (info.Flags & SHADER_RESOURCE_VIEW_FLAG_RAW_BUFFER)
					desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
				else
					desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

				desc.Buffer.FirstElement = info.Buffer.StartElement;
				desc.Buffer.NumElements = info.Buffer.ElementCount;
				desc.Buffer.StructureByteStride = info.Buffer.ElementStrideBytes;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipLevels = info.Texture1D.MipLevels;
				desc.Texture1D.MostDetailedMip = info.Texture1D.MostDetailedMip;
				desc.Texture1D.ResourceMinLODClamp = info.Texture1D.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipLevels = info.Texture1DArray.MipLevels;
				desc.Texture1DArray.MostDetailedMip = info.Texture1DArray.MostDetailedMip;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;
				desc.Texture1DArray.ResourceMinLODClamp = info.Texture1DArray.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = info.Texture2D.MipLevels;
				desc.Texture2D.MostDetailedMip = info.Texture2D.MostDetailedMip;
				desc.Texture2D.PlaneSlice = info.Texture2D.PlaneSlice;
				desc.Texture2D.ResourceMinLODClamp = info.Texture2D.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipLevels = info.Texture2DArray.MipLevels;
				desc.Texture2DArray.MostDetailedMip = info.Texture2DArray.MostDetailedMip;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.PlaneSlice = info.Texture2DArray.PlaneSlice;
				desc.Texture2DArray.ResourceMinLODClamp = info.Texture2DArray.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = info.Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = info.Texture2DMSArray.FirstArraySlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = info.Texture3D.MipLevels;
				desc.Texture3D.MostDetailedMip = info.Texture3D.MostDetailedMip;
				desc.Texture3D.ResourceMinLODClamp = info.Texture3D.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels = info.TextureCube.MipLevels;
				desc.TextureCube.MostDetailedMip = info.TextureCube.MostDetailedMip;
				desc.TextureCube.ResourceMinLODClamp = info.TextureCube.MinLODClamp;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP_ARRAY)
			{
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = info.TextureCube.MipLevels;
				desc.TextureCubeArray.MostDetailedMip = info.TextureCubeArray.MostDetailedMip;
				desc.TextureCubeArray.First2DArrayFace = info.TextureCubeArray.First2DArrayFace;
				desc.TextureCubeArray.NumCubes = info.TextureCubeArray.CubeCount;
				desc.TextureCubeArray.ResourceMinLODClamp = info.TextureCubeArray.MinLODClamp;
			}


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateShaderResourceView(pD3D12Resource, nullptr, m_View.CpuDescriptor);
		}
	}
}

#endif