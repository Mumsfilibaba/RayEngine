#include "..\..\Include\DX11\DX11Shader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Shader::DX11Shader(IDevice* pDevice, const ShaderInfo& byteCode)
			: DXShaderBase(),
			m_Device(nullptr),
			m_VertexShader(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, byteCode);
		}



		/////////////////////////////////////////////////////////////
		DX11Shader::~DX11Shader()
		{
			D3DRelease_S(m_VertexShader);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11VertexShader* DX11Shader::GetD3D11VertexShader() const
		{
			return m_VertexShader;
		}



		/////////////////////////////////////////////////////////////
		ID3D11HullShader* DX11Shader::GetD3D11HullShader() const
		{
			return m_HullShader;
		}



		/////////////////////////////////////////////////////////////
		ID3D11DomainShader* DX11Shader::GetD3D11DomainShader() const
		{
			return m_DomainShader;
		}



		/////////////////////////////////////////////////////////////
		ID3D11GeometryShader* DX11Shader::GetD3D11GeometryShader() const
		{
			return m_GeometryShader;
		}



		/////////////////////////////////////////////////////////////
		ID3D11PixelShader* DX11Shader::GetD3D11PixelShader() const
		{
			return m_PixelShader;
		}
		


		/////////////////////////////////////////////////////////////
		ID3D11ComputeShader* DX11Shader::GetD3D11ComputeShader() const
		{
			return m_ComputeShader;
		}



		/////////////////////////////////////////////////////////////
		SHADER_TYPE DX11Shader::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11Shader::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX11Shader::Create(IDevice* pDevice, const ShaderInfo& info)
		{
			using namespace System;

			int32 flags = 0;
			if (info.Flags & SHADER_FLAGS_DEBUG)
				flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

			bool result = false;
			if (info.FilePath.size() < 1)
				CompileFromString(info.Source, info.EntryPoint, info.Type, );
			else
				CompileFromFile(info.Source, info.FilePath, info.EntryPoint);

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = 0;

			const void* buffer = m_ShaderBlob->GetBufferPointer();
			int32 size = m_ShaderBlob->GetBufferSize();

			if (m_Type == SHADER_TYPE_VERTEX)
				hr = pD3D11Device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), size, nullptr, &m_VertexShader);
			else if (m_Type == SHADER_TYPE_HULL)
				hr = pD3D11Device->CreateHullShader(buffer, size, nullptr, &m_HullShader);
			else if (m_Type == SHADER_TYPE_DOMAIN)
				hr = pD3D11Device->CreateDomainShader(buffer, size, nullptr, &m_DomainShader);
			else if (m_Type == SHADER_TYPE_GEOMETRY)
				hr = pD3D11Device->CreateGeometryShader(buffer, size, nullptr, &m_GeometryShader);
			else if (m_Type == SHADER_TYPE_PIXEL)
				hr = pD3D11Device->CreatePixelShader(buffer, size, nullptr, &m_PixelShader);
			else if (m_Type == SHADER_TYPE_COMPUTE)
				hr = pD3D11Device->CreateComputeShader(buffer, size, nullptr, &m_ComputeShader);

			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create shader" + DXErrorString(hr));
				return;
			}
		}
	}
}

#endif
