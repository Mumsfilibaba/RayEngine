#include "..\..\Include\DX11\DX11Shader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11ShaderConstantBlock.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Shader::DX11Shader(IDevice* pDevice, const ShaderInfo& info)
			: DXShaderBase(),
			m_Device(nullptr),
			m_VertexShader(nullptr),
			m_ConstantBlock(nullptr),
			m_StaticSamplers()
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11Shader::~DX11Shader()
		{
			D3DRelease_S(m_VertexShader);
			for (int32 i = 0; i < m_StaticSamplers.size(); i++)
			{
				D3DRelease_S(m_StaticSamplers[i]);
			}

			ReRelease_S(m_ConstantBlock);
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11SamplerState* const * DX11Shader::GetStaticSamplers() const
		{
			return m_StaticSamplers.data();
		}



		/////////////////////////////////////////////////////////////
		int32 DX11Shader::GetStaticSamplerCount() const
		{
			return static_cast<int32>(m_StaticSamplers.size());
		}



		/////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock* DX11Shader::GetConstantBlock() const
		{
			return m_ConstantBlock;
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
		void DX11Shader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11Shader::Create(const ShaderInfo& info)
		{
			using namespace System;

			int32 flags = 0;
			if (info.Flags & SHADER_FLAGS_DEBUG)
				flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

			bool result = false;
			std::string errorString;

			if (info.FilePath.size() < 2)
				result = CompileFromString(info.Source, info.EntryPoint, info.Type, flags, errorString);
			else
				result = CompileFromFile(info.Source, info.FilePath, info.EntryPoint, info.Type, flags, errorString);

			if (!result)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not compile shader. " + errorString);
				return;
			}



			const void* buffer = m_ShaderBlob->GetBufferPointer();
			int32 size = static_cast<int32>(m_ShaderBlob->GetBufferSize());

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = 0;
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
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create shader" + DXErrorString(hr));
				return;
			}
			else
			{
				ID3D11DeviceChild* pD3D11DeviceChild = reinterpret_cast<ID3D11DeviceChild*>(m_VertexShader);
				pD3D11DeviceChild->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());

				CreateConstantBlock(info);
			}



			for (int32 i = 0; i < info.SamplerCount; i++)
			{
				ID3D11SamplerState* sampler = CreateSampler(info.pSamplers[i]);
				if (sampler == nullptr)
				{
					return;
				}
				else 
				{
					std::string name = info.Name + ": Static SamplerState " + std::to_string(i + 1);
					sampler->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());

					m_StaticSamplers.push_back(sampler);
				}
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11Shader::CreateConstantBlock(const ShaderInfo& info)
		{
			int32 numConstants = 0;
			for (int32 i = 0; i < info.VariableCount; i++)
			{
				if (info.pVariables[i].Type == VARIABLE_TYPE_SHADER_CONSTANTS)
				{
					numConstants = info.pVariables[i].ConstantCount;
					break;
				}
			}


			m_ConstantBlock = new DX11ShaderConstantBlock(m_Device, numConstants);
		}



		/////////////////////////////////////////////////////////////
		ID3D11SamplerState* DX11Shader::CreateSampler(const StaticSampler& sampler)
		{
			using namespace System;

			D3D11_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX11TextureAdressMode(sampler.AdressU);
			desc.AddressV = ReToDX11TextureAdressMode(sampler.AdressV);
			desc.AddressW = ReToDX11TextureAdressMode(sampler.AdressW);
			desc.ComparisonFunc = ReToDX11ComparisonFunc(sampler.ComparisonFunc);
			desc.Filter = ReToDX11Filter(sampler.FilterMode);
			desc.MaxAnisotropy = sampler.MaxAnistropy;
			desc.MinLOD = sampler.MinLOD;
			desc.MaxLOD = sampler.MaxLOD;
			desc.MipLODBias = sampler.MipLODBias;
			
			if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK)
			{
				desc.BorderColor[0] = 0.0f;
				desc.BorderColor[1] = 0.0f;
				desc.BorderColor[2] = 0.0f;
				desc.BorderColor[3] = 0.0f;
			}
			else if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK)
			{
				desc.BorderColor[0] = 0.0f;
				desc.BorderColor[1] = 0.0f;
				desc.BorderColor[2] = 0.0f;
				desc.BorderColor[3] = 1.0f;
			}
			else if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE)
			{
				desc.BorderColor[0] = 1.0f;
				desc.BorderColor[1] = 1.0f;
				desc.BorderColor[2] = 1.0f;
				desc.BorderColor[3] = 1.0f;
			}


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3D11SamplerState* pD3D11Sampler = nullptr;
			HRESULT hr = pD3D11Device->CreateSamplerState(&desc, &pD3D11Sampler);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create static SamplerState." + DXErrorString(hr));
				return nullptr;
			}

			return pD3D11Sampler;
		}
	}
}

#endif
