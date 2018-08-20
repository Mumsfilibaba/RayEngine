#pragma once

#include <vector>
#include "..\Graphics\IRootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class IDX12RootVaribleSlot;



		/////////////////////////////////////////////////////////////
		class DX12RootLayout : public IRootLayout
		{
		public:
			DX12RootLayout(const DX12RootLayout& other) = delete;
			DX12RootLayout& operator=(const DX12RootLayout& other) = delete;
			DX12RootLayout(DX12RootLayout&& other) = delete;
			DX12RootLayout& operator=(DX12RootLayout&& other) = delete;

		public:
			DX12RootLayout(IDevice* pDevice, const RootLayoutInfo& info);
			~DX12RootLayout();

			ID3D12RootSignature* GetD3D12RootSignature() const;
			
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(IDevice* pDevice, const RootLayoutInfo& info);
			D3D12_ROOT_PARAMETER CreateVariable(const ShaderVariable& variable);
			D3D12_STATIC_SAMPLER_DESC CreateSampler(const StaticSampler& sampler);
			IDX12RootVaribleSlot* CreateRootVariableSlot(const ShaderVariable& variable);

		private:
			DX12Device* m_Device;
			ID3D12RootSignature* m_RootSignature;
			std::vector<IDX12RootVaribleSlot> m_VariableSlots;
		};
	}
}

#endif