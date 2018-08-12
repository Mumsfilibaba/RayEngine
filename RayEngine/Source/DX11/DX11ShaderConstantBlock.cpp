#include "..\..\Include\DX11\DX11ShaderConstantBlock.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock::DX11ShaderConstantBlock(DX11Device* pDevice, int32 numConstants)
			: m_Device(nullptr),
			m_ConstantBuffer(nullptr),
			m_Constants()
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(numConstants);
		}



		/////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock::~DX11ShaderConstantBlock()
		{
			D3DRelease_S(m_ConstantBuffer);

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::SetConstant(uint32 constant, uint32 index)
		{
			m_Constants[index] = constant;
		}



		/////////////////////////////////////////////////////////////
		ID3D11Buffer* DX11ShaderConstantBlock::GetD3D11Buffer() const
		{
			return m_ConstantBuffer;
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::Create(int32 numConstants)
		{
			using namespace System;

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(uint32) * numConstants;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = sizeof(uint32);
			desc.Usage = D3D11_USAGE_DEFAULT;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBuffer(&desc, nullptr, &m_ConstantBuffer);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Buffer. " + DXErrorString(hr));
			}
			else
			{
				m_Constants.resize(numConstants);
			}
		}
	}
}

#endif