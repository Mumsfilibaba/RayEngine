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
#include "DX11/DX11RootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11/DX11Device.h"
#include "DX11/DX11RootVariableSlot.h"
#include "DX11/DX11ShaderConstantBlock.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11RootLayout::DX11RootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc)
			: m_Device(nullptr),
			m_ConstantBlocks(),
			m_StaticSamplers(),
			m_VariableSlots(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11RootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11RootLayout::GetDesc(RootLayoutDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11RootLayout::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11RootLayout::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11RootLayout::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11RootLayout::Create(const RootLayoutDesc* pDesc)
		{
			for (int32 i = 0; i < pDesc->SamplerCount; i++)
			{
				ID3D11SamplerState* pD3D11Sampler = CreateStaticSampler(&pDesc->pStaticSamplers[i]);
				if (pD3D11Sampler != nullptr)
				{
					m_StaticSamplers.push_back(pD3D11Sampler);
				}
				else
				{
					return;
				}
			}


			for (int32 i = 0; i < pDesc->VariableCount; i++)
			{
				if (pDesc->pVariables[i].Type == VARIABLE_TYPE_SHADER_CONSTANTS)
					m_ConstantBlocks.push_back(CreateConstantBlock(&pDesc->pVariables[i]));


				m_VariableSlots.push_back(CreateVariable(&pDesc->pVariables[i]));
			}


			m_Desc = *pDesc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11SamplerState* DX11RootLayout::CreateStaticSampler(const StaticSamplerDesc* pSampler)
		{
			D3D11_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX11TextureAdressMode(pSampler->AdressU);
			desc.AddressV = ReToDX11TextureAdressMode(pSampler->AdressV);
			desc.AddressW = ReToDX11TextureAdressMode(pSampler->AdressW);
			desc.ComparisonFunc = ReToDX11ComparisonFunc(pSampler->ComparisonFunc);
			desc.Filter = ReToDX11Filter(pSampler->FilterMode);
			desc.MaxAnisotropy = pSampler->MaxAnistropy;
			desc.MinLOD = pSampler->MinLOD;
			desc.MaxLOD = pSampler->MaxLOD;
			desc.MipLODBias = pSampler->MipLODBias;

			if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK)
			{
				desc.BorderColor[0] = 0.0f;
				desc.BorderColor[1] = 0.0f;
				desc.BorderColor[2] = 0.0f;
				desc.BorderColor[3] = 0.0f;
			}
			else if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK)
			{
				desc.BorderColor[0] = 0.0f;
				desc.BorderColor[1] = 0.0f;
				desc.BorderColor[2] = 0.0f;
				desc.BorderColor[3] = 1.0f;
			}
			else if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE)
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
				LOG_ERROR("D3D11: Could not create static SamplerState." + DXErrorString(hr));
				return nullptr;
			}

			return pD3D11Sampler;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IDX11RootVariableSlot* DX11RootLayout::CreateVariable(const ShaderVariableDesc* pVariable)
		{
			switch (pVariable->ShaderStage)
			{
			case SHADER_TYPE_VERTEX: 
				new DX11RootVariableSlotImpl<ID3D11VertexShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_HULL:
				new DX11RootVariableSlotImpl<ID3D11HullShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_DOMAIN:
				new DX11RootVariableSlotImpl<ID3D11DomainShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_GEOMETRY:
				new DX11RootVariableSlotImpl<ID3D11GeometryShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_PIXEL:
				new DX11RootVariableSlotImpl<ID3D11PixelShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_COMPUTE:
				new DX11RootVariableSlotImpl<ID3D11ComputeShader>(pVariable->ShaderRegister);
			case SHADER_TYPE_UNKNOWN:
			default: return nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11ShaderConstantBlock* DX11RootLayout::CreateConstantBlock(const ShaderVariableDesc* pVariable)
		{
			return new DX11ShaderConstantBlock(m_Device, pVariable->ConstantCount);
		}
	}
}

#endif