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

#include "RayEngine.h"
#include "DX11/DX11Shader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11/DX11Device.h"
#include "DX11/DX11ShaderConstantBlock.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Shader::DX11Shader(IDevice* pDevice, const ShaderDesc* pDesc)
			: DXShaderBase(),
			m_Device(nullptr),
			m_Shader(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Shader::~DX11Shader()
		{
			D3DRelease_S(m_Shader);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Shader::SetName(const std::string& name)
		{
			m_Shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Shader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Shader::GetDesc(ShaderDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Shader::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Shader::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Shader::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Shader::Create(const ShaderDesc* pDesc)
		{
			int32 flags = 0;
			if (pDesc->Flags & SHADER_FLAGS_DEBUG)
				flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;


			std::string errorString;
			if (!CompileFromString(pDesc->Source, pDesc->EntryPoint, pDesc->Type, flags, errorString))
			{
				LOG_ERROR("D3D11: Could not compile shader. " + errorString);
				return;
			}


			const void* buffer = m_ShaderBlob->GetBufferPointer();
			int32 size = static_cast<int32>(m_ShaderBlob->GetBufferSize());

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = 0;
			if (m_Type == SHADER_TYPE_VERTEX)
			{
				ID3D11VertexShader* pVS = nullptr;
				hr = pD3D11Device->CreateVertexShader(buffer, size, nullptr, &pVS);

				m_Shader = pVS;
			}
			else if (m_Type == SHADER_TYPE_HULL)
			{
				ID3D11HullShader* pHS = nullptr;
				hr = pD3D11Device->CreateHullShader(buffer, size, nullptr, &pHS);

				m_Shader = pHS;
			}
			else if (m_Type == SHADER_TYPE_DOMAIN)
			{
				ID3D11DomainShader* pDS = nullptr;
				hr = pD3D11Device->CreateDomainShader(buffer, size, nullptr, &pDS);

				m_Shader = pDS;
			}
			else if (m_Type == SHADER_TYPE_GEOMETRY)
			{
				ID3D11GeometryShader* pGS = nullptr;
				hr = pD3D11Device->CreateGeometryShader(buffer, size, nullptr, &pGS);

				m_Shader = pGS;
			}
			else if (m_Type == SHADER_TYPE_PIXEL)
			{
				ID3D11PixelShader* pPS = nullptr;
				hr = pD3D11Device->CreatePixelShader(buffer, size, nullptr, &pPS);

				m_Shader = pPS;
			}
			else if (m_Type == SHADER_TYPE_COMPUTE)
			{
				ID3D11ComputeShader* pCS = nullptr;
				hr = pD3D11Device->CreateComputeShader(buffer, size, nullptr, &pCS);

				m_Shader = pCS;
			}


			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create shader" + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;

				m_Shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());
			}
		}
	}
}

#endif