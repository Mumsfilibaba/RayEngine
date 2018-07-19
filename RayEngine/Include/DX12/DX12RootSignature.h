#pragma once

#include "..\Graphics\IRootSignature.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12RootSignature : public IRootSignature
		{
		public:
			DX12RootSignature(const DX12RootSignature& other) = delete;
			DX12RootSignature& operator=(const DX12RootSignature& other) = delete;

		public:
			DX12RootSignature(ID3D12Device* device, const RootSignatureInfo& info);
			DX12RootSignature(DX12RootSignature&& other);
			~DX12RootSignature();

			ID3D12RootSignature* GetRootSignature() const;

			DX12RootSignature& operator=(DX12RootSignature&& other);

		private:
			void Create(ID3D12Device* device, const RootSignatureInfo& info);

		private:
			ID3D12RootSignature* m_RootSignature;
		};
	}
}