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

#include <string>
#include <vector>
#include "..\..\Include\DX12\DX12RootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12RootVariableSlot.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RootLayout::DX12RootLayout(IDevice* pDevice, const RootLayoutInfo& info)
			: m_Device(nullptr),
			m_RootSignature(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12RootLayout::~DX12RootLayout()
		{
			D3DRelease_S(m_RootSignature);
		}



		/////////////////////////////////////////////////////////////
		ID3D12RootSignature* DX12RootLayout::GetD3D12RootSignature() const
		{
			return m_RootSignature;
		}



		/////////////////////////////////////////////////////////////
		DX12RootVariableSlot* DX12RootLayout::GetDX12RootVariableSlot(int32 index) const
		{
			return m_VariableSlots[index];
		}



		/////////////////////////////////////////////////////////////
		DX12RootVariableSlot* const * DX12RootLayout::GetDX12RootVariableSlotArray(int32 index) const
		{
			return m_VariableSlots.data();
		}



		/////////////////////////////////////////////////////////////
		int32 DX12RootLayout::GetDX12RootVariableSlotCount() const
		{
			return static_cast<int32>(m_VariableSlots.size());
		}



		/////////////////////////////////////////////////////////////
		void DX12RootLayout::SetName(const std::string& name)
		{
			D3D12SetName(m_RootSignature, name);
		}



		/////////////////////////////////////////////////////////////
		void DX12RootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RootLayout::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		void DX12RootLayout::Create(const RootLayoutInfo& info)
		{
			using namespace System;
			using namespace Microsoft::WRL;

			D3D12_FEATURE_DATA_ROOT_SIGNATURE feature = {};
			feature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &feature, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Root Signature not supported by graphics driver. " + DXErrorString(hr));
				return;
			}


			std::vector<D3D12_ROOT_PARAMETER1> params;
			params.resize(info.VariableCount);


			//TODO: Different types
			//TODO: Multiple Descriptors
			int32 usedShaders = 0;
			m_VariableSlots.resize(info.VariableCount);
			

			for (int32 i = 0; i < info.VariableCount; i++)
			{
				D3D12_ROOT_PARAMETER1 parameter = CreateVariable(info.pVariables[i]);
				params[i] = parameter;

				usedShaders |= parameter.ShaderVisibility;

				DX12RootVariableSlot* slot = CreateRootVariableSlot(info.pVariables[i], i, info.pVariables[i].ShaderUsage == SHADER_USAGE_STATIC);
				m_VariableSlots[i] = slot;
			}


			std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
			staticSamplers.resize(info.SamplerCount);

			for (int32 i = 0; i < info.SamplerCount; i++)
			{
				D3D12_STATIC_SAMPLER_DESC sampler = CreateSampler(info.pStaticSamplers[i]);
				staticSamplers[i] = sampler;
			}

			D3D12_ROOT_SIGNATURE_DESC1 desc = {};
			desc.NumParameters = static_cast<uint32>(params.size());
			desc.pParameters = params.data();
			desc.NumStaticSamplers = static_cast<uint32>(staticSamplers.size());
			desc.pStaticSamplers = staticSamplers.data();

			if (!(info.Flags & ROOT_LAYOUT_FLAG_DISABLE_INPUT_LAYOUT))
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
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not serialize RootSignature. " + err);
				return;
			}


			hr = pD3D12Device->CreateRootSignature(0, rSign->GetBufferPointer(), rSign->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create RootSignature" + DXErrorString(hr));
				return;
			}
			else
			{
				D3D12SetName(m_RootSignature, info.Name);
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_ROOT_PARAMETER1 DX12RootLayout::CreateVariable(const ShaderVariable& variable)
		{
			D3D12_ROOT_PARAMETER1 desc = {};
			D3D12_DESCRIPTOR_RANGE1 range = {};

			if (variable.Type == VARIABLE_TYPE_SAMPLER || variable.ShaderUsage == SHADER_USAGE_STATIC)
			{
				desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

				//TODO: Multiple Descriptors
				range.NumDescriptors = 1;

				//TODO: Flags may be different
				range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
				range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				range.RegisterSpace = variable.ShaderSpace;
				range.BaseShaderRegister = variable.ShaderRegister;

				if (variable.Type == VARIABLE_TYPE_UNIFORMBUFFER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				else if (variable.Type == VARIABLE_TYPE_TEXTURE)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				else if (variable.Type == VARIABLE_TYPE_UNORDERED_ACCESS)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
				else if (variable.Type == VARIABLE_TYPE_SAMPLER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;


				desc.DescriptorTable.NumDescriptorRanges = 1;
				desc.DescriptorTable.pDescriptorRanges = &range;
			}
			else if (variable.Type != VARIABLE_TYPE_SHADER_CONSTANTS || variable.ShaderUsage == SHADER_USAGE_DYNAMIC)
			{
				if (variable.Type == VARIABLE_TYPE_UNIFORMBUFFER)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				else if (variable.Type == VARIABLE_TYPE_TEXTURE)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
				else if (variable.Type == VARIABLE_TYPE_TEXTURE)
					desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;

				desc.Descriptor.ShaderRegister = variable.ShaderRegister;
				desc.Descriptor.RegisterSpace = variable.ShaderSpace;
				//TODO: Different flags
				desc.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC;
			}
			else if (variable.Type == VARIABLE_TYPE_SHADER_CONSTANTS)
			{
			}


			if (variable.ShaderStage == SHADER_TYPE_VERTEX)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			else if (variable.ShaderStage == SHADER_TYPE_HULL)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
			else if (variable.ShaderStage == SHADER_TYPE_DOMAIN)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
			else if (variable.ShaderStage == SHADER_TYPE_GEOMETRY)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
			else if (variable.ShaderStage == SHADER_TYPE_PIXEL)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			else if (variable.ShaderStage == SHADER_TYPE_COMPUTE)
				desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

			return desc;
		}



		/////////////////////////////////////////////////////////////
		D3D12_STATIC_SAMPLER_DESC DX12RootLayout::CreateSampler(const StaticSampler& sampler)
		{
			D3D12_STATIC_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX12TextureAdressMode(sampler.AdressU);
			desc.AddressV = ReToDX12TextureAdressMode(sampler.AdressV);
			desc.AddressW = ReToDX12TextureAdressMode(sampler.AdressW);
			desc.ComparisonFunc = ReToDX12ComparisonFunc(sampler.ComparisonFunc);
			desc.Filter = ReToDX12Filter(sampler.FilterMode);
			desc.MaxAnisotropy = sampler.MaxAnistropy;
			desc.MipLODBias = sampler.MipLODBias;
			desc.MinLOD = sampler.MinLOD;
			desc.MaxLOD = sampler.MaxLOD;
			desc.ShaderVisibility = ReShaderTypeToDX12ShaderVisibility(sampler.ShaderStage);

			if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			else if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
			else if (sampler.BorderColor == STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE)
				desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;

			desc.RegisterSpace = sampler.ShaderSpace;
			desc.ShaderRegister = sampler.ShaderRegister;

			return desc;
		}



		/////////////////////////////////////////////////////////////
		DX12RootVariableSlot* DX12RootLayout::CreateRootVariableSlot(const ShaderVariable& variable, int32 rootSlot, bool placeDescriptorTable)
		{
			if (variable.ShaderStage == SHADER_TYPE_COMPUTE)
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