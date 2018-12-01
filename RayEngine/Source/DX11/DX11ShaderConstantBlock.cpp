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

#include "../../Include/Debug/Debug.h"
#include "../../Include/DX11/DX11ShaderConstantBlock.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX11/DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock::DX11ShaderConstantBlock(DX11Device* pDevice, int32 numConstants)
			: m_Device(nullptr),
			m_ConstantBuffer(nullptr),
			m_Constants(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(numConstants);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock::~DX11ShaderConstantBlock()
		{
			D3DRelease_S(m_ConstantBuffer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::SetConstant(uint32 constant, uint32 index)
		{
			m_Constants[index] = constant;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Buffer* DX11ShaderConstantBlock::GetD3D11Buffer() const
		{
			return m_ConstantBuffer;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::SetName(const std::string& name)
		{
			m_ConstantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderConstantBlock::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderConstantBlock::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderConstantBlock::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderConstantBlock::Create(int32 numConstants)
		{
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
				LOG_ERROR("D3D11: Could not create Buffer. " + DXErrorString(hr));
			}
			else
			{
				m_Constants.resize(numConstants);
			}
		}
	}
}

#endif