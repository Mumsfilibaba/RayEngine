#include <string>
#include <vector>
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12RootSignature.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RootSignature::DX12RootSignature(const IDevice* pDevice, const RootSignatureInfo& info)
			: m_RootSignature(nullptr)
		{
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12RootSignature::DX12RootSignature(DX12RootSignature&& other)
			: m_RootSignature(other.m_RootSignature)
		{
			other.m_RootSignature = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12RootSignature::~DX12RootSignature()
		{
			D3DRelease_S(m_RootSignature);
		}



		/////////////////////////////////////////////////////////////
		ID3D12RootSignature* DX12RootSignature::GetD3D12RootSignature() const
		{
			return m_RootSignature;
		}



		/////////////////////////////////////////////////////////////
		DX12RootSignature& DX12RootSignature::operator=(DX12RootSignature&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_RootSignature);

				m_RootSignature = other.m_RootSignature;
				other.m_RootSignature = nullptr;
			}

			return *this;
		}

		IReferenceCounter * DX12RootSignature::QueryReference()
		{
			return nullptr;
		}

		uint32 DX12RootSignature::GetReferenceCount() const
		{
			return uint32();
		}

		void DX12RootSignature::Release() const
		{
		}

		uint32 DX12RootSignature::AddRef()
		{
			return uint32();
		}



		/////////////////////////////////////////////////////////////
		void DX12RootSignature::Create(const IDevice* pDevice, const RootSignatureInfo& info)
		{
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
				
				if (info.pParameters[i].ViewType == VIEW_TYPE_UNIFORMBUFFER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				else if (info.pParameters[i].ViewType == VIEW_TYPE_TEXTURE)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				else if (info.pParameters[i].ViewType == VIEW_TYPE_SAMPLER)
					range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;


				D3D12_ROOT_PARAMETER1 parameter = {};
				parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				parameter.DescriptorTable = { 1, &range };


				if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_ALL)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_VERTEX)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
				else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_HULL)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_HULL;
				else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_DOMAIN)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
				else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_GEOMETRY)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
				else if (info.pParameters[i].ShaderVisibility == SHADER_VISIBILITY_PIXEL)
					parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


				params.push_back(parameter);
			}


			D3D12_ROOT_SIGNATURE_DESC1 rDesc = {};
			memset(&rDesc, 0, sizeof(D3D12_ROOT_SIGNATURE_DESC));

			rDesc.NumParameters = static_cast<uint32>(params.size());
			rDesc.pParameters = params.data();

			//TODO: Static samplers

			if (info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_INPUT_LAYOUT)
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_VERTEX_SHADER))
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;

			if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_HULL_SHADER))
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

			if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_DOMAIN_SHADER))
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

			if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_GEOMETRY_SHADER))
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

			if (!(info.RootSignatureVisibility & ROOT_SIGNATURE_VISIBILITY_PIXEL_SHADER))
				rDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

			
			D3D12_VERSIONED_ROOT_SIGNATURE_DESC vrsDesc = {};
			vrsDesc.Version = feature.HighestVersion;
			vrsDesc.Desc_1_1 = rDesc;


			ComPtr<ID3DBlob> error;
			ComPtr<ID3DBlob> rSign;
			if (FAILED(D3D12SerializeVersionedRootSignature(&vrsDesc, &rSign, &error)))
			{
				std::string err = reinterpret_cast<char*>(error->GetBufferPointer());
				return;
			}


			if (FAILED(pD3D12Device->CreateRootSignature(0, rSign->GetBufferPointer(), 
				rSign->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature))))
			{
				return;
			}
			else
			{
				D3D12SetName(m_RootSignature, info.Name);
			}
		}
	}
}