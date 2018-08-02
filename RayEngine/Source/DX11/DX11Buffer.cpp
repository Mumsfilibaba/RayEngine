#include "..\..\Include\DX11\DX11Buffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Buffer::DX11Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_ByteStride(0),
			m_ReferenceCounter(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, pInitalData, info);
		}



		/////////////////////////////////////////////////////////////
		DX11Buffer::~DX11Buffer()
		{
			D3DRelease_S(m_Resource);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		ID3D11Buffer* DX11Buffer::GetD3D11Buffer() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		int32 DX11Buffer::GetByteStride() const
		{
			return m_ByteStride;
		}



		/////////////////////////////////////////////////////////////
		void* DX11Buffer::Map(int32 subresource)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			ID3D11DeviceContext* pD3D11Context = reinterpret_cast<DX11Device*>(m_Device)->GetD3D11DeviceContext();
			pD3D11Context->Map(m_Resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			return mappedResource.pData;
		}



		/////////////////////////////////////////////////////////////
		void DX11Buffer::Unmap()
		{
			ID3D11DeviceContext* pD3D11Context = reinterpret_cast<DX11Device*>(m_Device)->GetD3D11DeviceContext();
			pD3D11Context->Unmap(m_Resource, 0);
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11Buffer::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11Buffer::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Buffer::GetReferenceCount() const
		{
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Buffer::Release() const
		{
			m_ReferenceCounter--;
			if (m_ReferenceCounter < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Buffer::AddRef()
		{
			m_ReferenceCounter++;
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Buffer::Create(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
		{
			using namespace System;

			D3D11_BUFFER_DESC desc = {};
			desc.ByteWidth = info.ByteStride * info.Count;

			desc.BindFlags = 0;
			if (info.Type == BUFFER_USAGE_VERTEX)
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			else if (info.Type == BUFFER_USAGE_INDEX)
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			else if (info.Type == BUFFER_USAGE_UNIFORM)
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			
			desc.CPUAccessFlags = 0;
			if (info.CpuAccess & CPU_ACCESS_FLAG_WRITE)
				desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			if (info.CpuAccess & CPU_ACCESS_FLAG_READ)
				desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;

			desc.MiscFlags = 0;
			desc.StructureByteStride = m_ByteStride = info.ByteStride;
			
			if (info.Usage == RESOURCE_USAGE_DEFAULT)
				desc.Usage = D3D11_USAGE_DEFAULT;
			else if (info.Usage == RESOURCE_USAGE_DYNAMIC)
				desc.Usage = D3D11_USAGE_DYNAMIC;
			else if (info.Usage == RESOURCE_USAGE_STATIC)
				desc.Usage = D3D11_USAGE_IMMUTABLE;


			D3D11_SUBRESOURCE_DATA* pInitData = nullptr;
			D3D11_SUBRESOURCE_DATA initData = {};
			if (pInitalData != nullptr)
			{
				initData.pSysMem = pInitalData->pData;
				pInitData = &initData;
			}


			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBuffer(&desc, pInitData, &m_Resource);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create buffer. " + DXErrorString(hr));
				return;
			}
		}
	}
}

#endif