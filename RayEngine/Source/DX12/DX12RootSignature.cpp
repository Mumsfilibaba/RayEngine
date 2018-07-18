#include "..\..\Include\DX12\DX12RootSignature.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RootSignature::DX12RootSignature(ID3D12Device* device, const RootSignatureInfo& info)
			: m_RootSignature(nullptr)
		{
			Create(device, info);
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



		/////////////////////////////////////////////////////////////
		void DX12RootSignature::Create(ID3D12Device* device, const RootSignatureInfo& info)
		{
		}
	}
}