#include "..\..\Include\DX11\DX11Shader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Shader::DX11Shader(IDevice* pDevice, const ShaderByteCode& byteCode)
			: m_Device(nullptr),
			m_VertexShader(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN),
			m_ReferenceCount(0),
			m_ByteCode()
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, byteCode);
		}



		/////////////////////////////////////////////////////////////
		DX11Shader::DX11Shader(DX11Shader&& other)
			: m_Device(other.m_Device),
			m_VertexShader(other.m_VertexShader),
			m_Type(other.m_Type),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_VertexShader = nullptr;
			other.m_Type = SHADER_TYPE_UNKNOWN;
			other.m_ReferenceCount = 0;

			m_ByteCode = std::move(other.m_ByteCode);
		}



		/////////////////////////////////////////////////////////////
		DX11Shader::~DX11Shader()
		{
			D3DRelease_S(m_VertexShader);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX11Shader& DX11Shader::operator=(DX11Shader&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_VertexShader);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_VertexShader = other.m_VertexShader;
				m_Type = other.m_Type;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_VertexShader = nullptr;
				other.m_Type = SHADER_TYPE_UNKNOWN;
				other.m_ReferenceCount = 0;

				
				m_ByteCode = std::move(other.m_ByteCode);
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		const ShaderByteCode& DX11Shader::GetByteCode() const
		{
			return m_ByteCode;
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
		IReferenceCounter* DX11Shader::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Shader::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Shader::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Shader::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Shader::Create(IDevice* pDevice, const ShaderByteCode& byteCode)
		{
			using namespace System;

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = 0;

			if (byteCode.GetType() == SHADER_TYPE_VERTEX)
				hr = pD3D11Device->CreateVertexShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_VertexShader);
			else if (byteCode.GetType() == SHADER_TYPE_HULL)
				hr = pD3D11Device->CreateHullShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_HullShader);
			else if (byteCode.GetType() == SHADER_TYPE_DOMAIN)
				hr = pD3D11Device->CreateDomainShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_DomainShader);
			else if (byteCode.GetType() == SHADER_TYPE_GEOMETRY)
				hr = pD3D11Device->CreateGeometryShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_GeometryShader);
			else if (byteCode.GetType() == SHADER_TYPE_PIXEL)
				hr = pD3D11Device->CreatePixelShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_PixelShader);
			else if (byteCode.GetType() == SHADER_TYPE_COMPUTE)
				hr = pD3D11Device->CreateComputeShader(byteCode.GetBytes(), byteCode.GetSize(), nullptr, &m_ComputeShader);

			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create shader" + DXErrorString(hr));
				return;
			}

			m_Type = byteCode.GetType();
			m_ByteCode = byteCode;
		}
	}
}

#endif
