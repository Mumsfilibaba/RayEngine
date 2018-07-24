#pragma once

#include "..\Graphics\IDevice.h"
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
			DX12RootSignature(const IDevice* pDevice, const RootSignatureInfo& info);
			DX12RootSignature(DX12RootSignature&& other);
			~DX12RootSignature();

			ID3D12RootSignature* GetD3D12RootSignature() const;

			DX12RootSignature& operator=(DX12RootSignature&& other);

		private:
			void Create(const IDevice* pDevice, const RootSignatureInfo& info);

		private:
			ID3D12RootSignature* m_RootSignature;
		};
	}
}