#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IRootSignature.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

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
			DX12RootSignature(IDevice* pDevice, const RootSignatureInfo& info);
			DX12RootSignature(DX12RootSignature&& other);
			~DX12RootSignature();

			ID3D12RootSignature* GetD3D12RootSignature() const;

			IDevice* GetDevice() const override final;

			DX12RootSignature& operator=(DX12RootSignature&& other);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const RootSignatureInfo& info);

		private:
			IDevice* m_Device;

			ID3D12RootSignature* m_RootSignature;
			
			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif