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

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12/DX12Device.h"
#include "DX12/DX12RootLayout.h"
#include "DX12/DX12RootVariableSlot.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12RootLayout::DX12RootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc)
			: m_Device(nullptr),
			m_RootSignature(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12RootLayout::~DX12RootLayout()
		{
			D3DRelease_S(m_RootSignature);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12RootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12RootLayout::GetDesc(RootLayoutDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12RootLayout::Create(const RootLayoutDesc* pDesc)
		{
			using namespace Microsoft::WRL;

			D3D12_FEATURE_DATA_ROOT_SIGNATURE feature = {};
			feature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &feature, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Root Signature not supported by graphics driver. " + DXErrorString(hr));
				return;
			}


			std::vector<D3D12_ROOT_PARAMETER1> params;
			params.resize(pDesc->VariableCount);


			//TODO: Different types
			//TODO: Multiple Descriptors
			int32 usedShaders = 0;
			m_VariableSlots.resize(pDesc->VariableCount);
			

			for (int32 i = 0; i < pDesc->VariableCount; i++)
			{
				D3D12_ROOT_PARAMETER1 parameter = CreateVariable(&pDesc->pVariables[i]);
				params[i] = parameter;

				usedShaders |= parameter.ShaderVisibility;

				DX12RootVariableSlot* slot = CreateRootVariableSlot(&pDesc->pVariables[i], i, pDesc->pVariables[i].ShaderUsage == SHADER_USAGE_STATIC);
				m_VariableSlots[i] = slot;
			}


			std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
			staticSamplers.resize(pDesc->SamplerCount);

			for (int32 i = 0; i < pDesc->SamplerCount; i++)
			{
				D3D12_STATIC_SAMPLER_DESC sampler = CreateSampler(&pDesc->pStaticSamplers[i]);
				staticSamplers[i] = sampler;
			}

			D3D12_ROOT_SIGNATURE_DESC1 desc = {};
			desc.NumParameters = static_cast<uint32>(params.size());
			desc.pParameters = params.data();
			desc.NumStaticSamplers = static_cast<uint32>(staticSamplers.size());
			desc.pStaticSamplers = staticSamplers.data();

			if (!(pDesc->Flags & ROOT_LAYOUT_FLAG_DISABLE_INPUT_LAYOUT))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			if (!(usedShaders & D3D12_SHADER_VISIBILITY_VERTEX))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
			if (!(usedShaders & D3D12_SHADER_VISIBILITY_HULL))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
			if (!(usedShaders & D3D12_SHADER_VISIBILITY_DOMAIN))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
			if (!(usedShaders & D3D12_SHADER_VISIBILITY_GEOMETRY))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
			if (!(usedShaders & D3D12_SHADER_VISIBILITY_PIXEL))
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

			
			D3D12_VERSIONED_ROOT_SIGNATURE_DESC vrsDesc = {};
			vrsDesc.Version = feature.HighestVersion;
			vrsDesc.Desc_1_1 = desc;


			ComPtr<ID3DBlob> error;
			ComPtr<ID3DBlob> rSign;
			hr = D3D12SerializeVersionedRootSignature(&vrsDesc, &rSign, &error);
			if (FAILED(hr))
			{
				std::string err = reinterpret_cast<char*>(error->GetBufferPointer());
				LOG_ERROR("D3D12: Could not serialize RootSignature. " + err);
				return;
			}


			hr = pD3D12Device->CreateRootSignature(0, rSign->GetBufferPointer(), rSign->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create RootSignature" + DXErrorString(hr));
				return;
			}
			else
			{
				m_Desc = *pDesc;
			}
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_ROOT_PARAMETER1 DX12RootLayout::CreateVariable(const ShaderVariableDesc* pVariable)
		{
			D3D12_ROOT_PARAMETER1 desc = {};
			D3D12_DESCRIPTOR_RANGE1 range = {};

			if (pVariable->Type == VARIABLE_TYPE_SAMPLER || pVariable->ShaderUsage == SHADER_USAGE_STATIC)
			{
				desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

				//TODO: Multiple Descriptors
				range.NumDescriptors = 1;

				//TODO: Flags may be different
				range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
				range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				range.RegisterSpace = pVariable->ShaderSpace;
				range.BaseShaderRegister = pVariable->ShaderRegister;

				if (pVariable->Type == VARIABLE_TYPE_UNIFORMBUFFER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				else if (pVariable->Type == VARIABLE_TYPE_TEXTURE)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				else if (pVariable->Type == VARIABLE_TYPE_UNORDERED_ACCESS)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
				else if (pVariable->Type == VARIABLE_TYPE_SAMPLER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;


				desc.DescriptorTable.NumDescriptorRanges = 1;
				desc.DescriptorTable.pDescriptorRanges = &range;
			}
			else if (pVariable->Type != VARIABLE_TYPE_SHADER_CONSTANTS || pVariable->ShaderUsage == SHADER_USAGE_DYNAMIC)
			{
				if (pVariable->Type == VARIABLE_TYPE_UNIFORMBUFFER)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				else if (pVariable->Type == VARIABLE_TYPE_TEXTURE)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
				else if (pVariable->Type == VARIABLE_TYPE_TEXTURE)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;

				desc.Descriptor.ShaderRegister = pVariable->ShaderRegister;
				desc.Descriptor.RegisterSpace = pVariable->ShaderSpace;
				//TODO: Different flags
				desc.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC;
			}
			else if (pVariable->Type == VARIABLE_TYPE_SHADER_CONSTANTS)
			{
			}


			if (pVariable->ShaderStage == SHADER_TYPE_VERTEX)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			else if (pVariable->ShaderStage == SHADER_TYPE_HULL)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
			else if (pVariable->ShaderStage == SHADER_TYPE_DOMAIN)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
			else if (pVariable->ShaderStage == SHADER_TYPE_GEOMETRY)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
			else if (pVariable->ShaderStage == SHADER_TYPE_PIXEL)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			else if (pVariable->ShaderStage == SHADER_TYPE_COMPUTE)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

			return desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_STATIC_SAMPLER_DESC DX12RootLayout::CreateSampler(const StaticSamplerDesc* pSampler)
		{
			D3D12_STATIC_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX12TextureAdressMode(pSampler->AdressU);
			desc.AddressV = ReToDX12TextureAdressMode(pSampler->AdressV);
			desc.AddressW = ReToDX12TextureAdressMode(pSampler->AdressW);
			desc.ComparisonFunc = ReToDX12ComparisonFunc(pSampler->ComparisonFunc);
			desc.Filter = ReToDX12Filter(pSampler->FilterMode);
			desc.MaxAnisotropy = pSampler->MaxAnistropy;
			desc.MipLODBias = pSampler->MipLODBias;
			desc.MinLOD = pSampler->MinLOD;
			desc.MaxLOD = pSampler->MaxLOD;
			desc.ShaderVisibility = ReShaderTypeToDX12ShaderVisibility(pSampler->ShaderStage);

			if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			else if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
			else if (pSampler->BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;

			desc.RegisterSpace = pSampler->ShaderSpace;
			desc.ShaderRegister = pSampler->ShaderRegister;

			return desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12RootVariableSlot* DX12RootLayout::CreateRootVariableSlot(const ShaderVariableDesc* pVariable, int32 rootSlot, bool placeDescriptorTable)
		{
			if (pVariable->ShaderStage == SHADER_TYPE_COMPUTE)
			{
				if (placeDescriptorTable)
					return new DX12ComputeDescriptorRootSlot(D3D12_RESOURCE_STATE_COMMON, rootSlot);
				else
					return new DX12ComputeRootSignatureSlot(D3D12_RESOURCE_STATE_COMMON, rootSlot);
			}
			else
			{
				if (placeDescriptorTable)
					return new DX12GraphicsDescriptorRootSlot(D3D12_RESOURCE_STATE_COMMON, rootSlot);
				else
					return new DX12GraphicsRootSignatureSlot(D3D12_RESOURCE_STATE_COMMON, rootSlot);
			}

			return nullptr;
		}
	}
}

#endif