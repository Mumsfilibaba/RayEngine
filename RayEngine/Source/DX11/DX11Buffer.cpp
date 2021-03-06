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

#include <RayEngine.h>
#include <DX11/DX11Buffer.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX11/DX11Device.h>
#include <DX11/DX11DeviceContext.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Buffer::DX11Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferDesc* pDesc)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_Context(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);
			m_Device->GetImmediateContext(reinterpret_cast<IDeviceContext**>(&m_Context));

			Create(pInitalData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Buffer::~DX11Buffer()
		{
			D3DRelease_S(m_Resource);

			ReRelease_S(m_Context);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void* DX11Buffer::Map(int32 subresource, RESOURCE_MAP_FLAG flag)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			D3D11_MAP map = {};
			if (RESOURCE_MAP_FLAG_READ)
				map = D3D11_MAP_READ;
			else if (RESOURCE_MAP_FLAG_READ)
				map = D3D11_MAP_WRITE;

			ID3D11DeviceContext* pD3D11Context = m_Context->GetD3D11DeviceContext();
			pD3D11Context->Map(m_Resource, 0, map, 0, &mappedResource);

			return mappedResource.pData;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Buffer::Unmap()
		{
			ID3D11DeviceContext* pD3D11Context = m_Context->GetD3D11DeviceContext();
			pD3D11Context->Unmap(m_Resource, 0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Buffer::GetDesc(BufferDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Buffer::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Buffer::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Buffer::Create(const ResourceData* pInitalData, const BufferDesc* pDesc)
		{
			D3D11_BUFFER_DESC desc = {};
			desc.ByteWidth = pDesc->ByteStride * pDesc->Count;

			desc.BindFlags = 0;
			if (pDesc->Type == BUFFER_USAGE_VERTEX)
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			else if (pDesc->Type == BUFFER_USAGE_INDEX)
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			else if (pDesc->Type == BUFFER_USAGE_CONSTANT)
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			
			desc.CPUAccessFlags = 0;
			if (pDesc->CpuAccess & CPU_ACCESS_FLAG_WRITE)
				desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			if (pDesc->CpuAccess & CPU_ACCESS_FLAG_READ)
				desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;

			desc.MiscFlags = 0;
			desc.StructureByteStride = pDesc->ByteStride;
			
			if (pDesc->Usage == RESOURCE_USAGE_DEFAULT)
				desc.Usage = D3D11_USAGE_DEFAULT;
			else if (pDesc->Usage == RESOURCE_USAGE_DYNAMIC)
				desc.Usage = D3D11_USAGE_DYNAMIC;
			else if (pDesc->Usage == RESOURCE_USAGE_STATIC)
				desc.Usage = D3D11_USAGE_IMMUTABLE;


			D3D11_SUBRESOURCE_DATA* pInitData = nullptr;
			D3D11_SUBRESOURCE_DATA initData = {};
			if (pInitalData != nullptr)
			{
				initData.pSysMem = pInitalData->pData;
				pInitData = &initData;
			}


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBuffer(&desc, pInitData, &m_Resource);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create buffer. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}

#endif