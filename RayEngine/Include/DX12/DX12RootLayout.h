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
		class IDX12RootVariableSlot;



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
			IDX12RootVariableSlot* GetDX12RootVariableSlot(int32 index) const;
			IDX12RootVariableSlot* const * GetDX12RootVariableSlotArray(int32 index) const;
			int32 GetDX12RootVariableSlotCount() const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const RootLayoutInfo& info);
			D3D12_ROOT_PARAMETER1 CreateVariable(const ShaderVariable& variable);
			D3D12_STATIC_SAMPLER_DESC CreateSampler(const StaticSampler& sampler);
			IDX12RootVariableSlot* CreateRootVariableSlot(const ShaderVariable& variable, int32 rootSlot);

		private:
			DX12Device* m_Device;
			ID3D12RootSignature* m_RootSignature;
			std::vector<IDX12RootVariableSlot*> m_VariableSlots;
		};
	}
}

#endif