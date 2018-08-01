#pragma once

#include "..\Graphics\IRootSignature.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11RootSignature : public IRootSignature
		{
		public:
			DX11RootSignature(const DX11RootSignature& other) = delete;
			DX11RootSignature& operator=(const DX11RootSignature& other) = delete;

		public:
			DX11RootSignature(IDevice* pDevice, const RootSignatureInfo& info);
			DX11RootSignature(DX11RootSignature&& other);
			~DX11RootSignature();

			IDevice* GetDevice() const override final;

			DX11RootSignature& operator=(DX11RootSignature&& other);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const RootSignatureInfo& info);

		private:
			IDevice* m_Device;
			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif