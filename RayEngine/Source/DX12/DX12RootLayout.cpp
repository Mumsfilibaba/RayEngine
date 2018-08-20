#include <string>
#include <vector>
#include "..\..\Include\DX12\DX12RootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RootLayout::DX12RootLayout(IDevice* pDevice, const RootLayoutInfo& info)
			: m_Device(nullptr),
			m_RootSignature(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12RootLayout::~DX12RootLayout()
		{
			D3DRelease_S(m_RootSignature);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D12RootSignature* DX12RootLayout::GetD3D12RootSignature() const
		{
			return m_RootSignature;
		}



		/////////////////////////////////////////////////////////////
		void DX12RootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12RootLayout::Create(IDevice* pDevice, const RootLayoutInfo& info)
		{
			using namespace System;
			using namespace Microsoft::WRL;

			D3D12_FEATURE_DATA_ROOT_SIGNATURE feature = {};
			feature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;


			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			if (FAILED(pD3D12Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &feature, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE))))
				return;


			std::vector<D3D12_ROOT_PARAMETER1> params;
			params.resize(info.ParameterCount);


			//TODO: Different types
			//TODO: Multiple Descriptors
			for (int32 i = 0; i < info.ParameterCount; i++)
			{
				D3D12_DESCRIPTOR_RANGE1 range = {};
				range.NumDescriptors = 1;
				range.RegisterSpace = 0;
				range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
				range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				range.BaseShaderRegister = info.pParameters[i].ShaderRegister;
				
				if (info.pParameters[i].Type == VARIABLE_TYPE_UNIFORMBUFFER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				else if (info.pParameters[i].Type == VARIABLE_TYPE_TEXTURE)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				else if (info.pParameters[i].Type == VARIABLE_TYPE_SAMPLER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;


				D3D12_ROOT_PARAMETER1 parameter = {};
				parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				parameter.DescriptorTable = { 1, &range };


				parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				//if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_ALL)
				//else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_VERTEX)
				//	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
				//else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_HULL)
				//	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
				//else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_DOMAIN)
				//	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
				//else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_GEOMETRY)
				//	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
				//else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_PIXEL)
				//	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


				params.push_back(parameter);
			}


			D3D12_ROOT_SIGNATURE_DESC1 rDesc = {};
			memset(&rDesc, 0, sizeof(D3D12_ROOT_SIGNATURE_DESC));

			rDesc.NumParameters = static_cast<uint32>(params.size());
			rDesc.pParameters = params.data();

			//TODO: Static samplers

			rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			//if (info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_INPUT_LAYOUT)
			//	

			//if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_VERTEX_SHADER))
			//	rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;

			//if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_HULL_SHADER))
			//	rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

			//if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_DOMAIN_SHADER))
			//	rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

			//if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_GEOMETRY_SHADER))
			//	rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

			//if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_PIXEL_SHADER))
			//	rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

			
			D3D12_VERSIONED_ROOT_SIGNATURE_DESC vrsDesc = {};
			vrsDesc.Version = feature.HighestVersion;
			vrsDesc.Desc_1_1 = rDesc;


			ComPtr<ID3DBlob> error;
			ComPtr<ID3DBlob> rSign;
			HRESULT hr = D3D12SerializeVersionedRootSignature(&vrsDesc, &rSign, &error);
			if (FAILED(hr))
			{
				std::string err = reinterpret_cast<char*>(error->GetBufferPointer());
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not serialize RootSignature" + err);
				return;
			}


			hr = pD3D12Device->CreateRootSignature(0, rSign->GetBufferPointer(), rSign->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create RootSignature" + DXErrorString(hr));
				return;
			}
			else
			{
				D3D12SetName(m_RootSignature, info.Name);
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_ROOT_PARAMETER DX12RootLayout::CreateVariable(const ShaderVariable& variable)
		{
			return D3D12_ROOT_PARAMETER();
		}



		/////////////////////////////////////////////////////////////
		D3D12_STATIC_SAMPLER_DESC DX12RootLayout::CreateSampler(const StaticSampler& sampler)
		{
			return D3D12_STATIC_SAMPLER_DESC();
		}



		/////////////////////////////////////////////////////////////
		IDX12RootVaribleSlot* DX12RootLayout::CreateRootVariableSlot(const ShaderVariable& variable)
		{
			return nullptr;
		}
	}
}

#endif