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

#include "..\..\Include\DX11\DX11RootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11RootVariableSlot.h"
#include "..\..\Include\DX11\DX11ShaderConstantBlock.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11RootLayout::DX11RootLayout(IDevice* pDevice, const RootLayoutInfo& info)
			: m_Device(nullptr),
			m_ConstantBlocks(),
			m_StaticSamplers(),
			m_VariableSlots()
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11RootLayout::~DX11RootLayout()
		{
			for (int32 i = 0; i < static_cast<int32>(m_StaticSamplers.size()); i++)
			{
				D3DRelease_S(m_StaticSamplers[i]);
			}

			for (int32 i = 0; i < static_cast<int32>(m_ConstantBlocks.size()); i++)
			{
				ReRelease_S(m_ConstantBlocks[i]);
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		IDX11RootVariableSlot* DX11RootLayout::GetDX11VariableSlot(int32 slotIndex) const
		{
			return m_VariableSlots[slotIndex];
		}



		/////////////////////////////////////////////////////////////
		void DX11RootLayout::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11RootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11RootLayout::Create(const RootLayoutInfo& info)
		{
			for (int32 i = 0; i < info.SamplerCount; i++)
			{
				ID3D11SamplerState* pD3D11Sampler = CreateStaticSampler(info.pStaticSamplers[i]);
				if (pD3D11Sampler != nullptr)
				{
					m_StaticSamplers.push_back(pD3D11Sampler);
				}
				else
				{
					return;
				}
			}



			for (int32 i = 0; i < info.VariableCount; i++)
			{
				if (info.pVariables[i].Type == VARIABLE_TYPE_SHADER_CONSTANTS)
					m_ConstantBlocks.push_back(CreateConstantBlock(info.pVariables[i]));


				m_VariableSlots.push_back(CreateVariable(info.pVariables[i]));
			}
		}



		/////////////////////////////////////////////////////////////
		ID3D11SamplerState* DX11RootLayout::CreateStaticSampler(const StaticSampler& sampler)
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



		/////////////////////////////////////////////////////////////
		IDX11RootVariableSlot* DX11RootLayout::CreateVariable(const ShaderVariable& variable)
		{
			switch (variable.ShaderStage)
			{
			case SHADER_TYPE_VERTEX: 
				new DX11RootVariableSlotImpl<ID3D11VertexShader>(variable.ShaderRegister);
			case SHADER_TYPE_HULL:
				new DX11RootVariableSlotImpl<ID3D11HullShader>(variable.ShaderRegister);
			case SHADER_TYPE_DOMAIN:
				new DX11RootVariableSlotImpl<ID3D11DomainShader>(variable.ShaderRegister);
			case SHADER_TYPE_GEOMETRY:
				new DX11RootVariableSlotImpl<ID3D11GeometryShader>(variable.ShaderRegister);
			case SHADER_TYPE_PIXEL:
				new DX11RootVariableSlotImpl<ID3D11PixelShader>(variable.ShaderRegister);
			case SHADER_TYPE_COMPUTE:
				new DX11RootVariableSlotImpl<ID3D11ComputeShader>(variable.ShaderRegister);
			case SHADER_TYPE_UNKNOWN:
			default: return nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock* DX11RootLayout::CreateConstantBlock(const ShaderVariable& variable)
		{
			return new DX11ShaderConstantBlock(m_Device, variable.ConstantCount);
		}
	}
}

#endif