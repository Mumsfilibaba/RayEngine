#pragma once

#include "..\Graphics\IRootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;
		class IDX11RootVariableSlot;



		/////////////////////////////////////////////////////////////
		class DX11RootLayout : public IRootLayout
		{
		public:
			DX11RootLayout(const DX11RootLayout& other) = delete;
			DX11RootLayout& operator=(const DX11RootLayout& other) = delete;
			DX11RootLayout(DX11RootLayout&& other) = delete;
			DX11RootLayout& operator=(DX11RootLayout&& other) = delete;

		public:
			DX11RootLayout(IDevice* pDevice, const RootLayoutInfo& info);
			~DX11RootLayout();

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const RootLayoutInfo& info);
			ID3D11SamplerState* CreateStaticSampler(const StaticSampler& sampler);
			IDX11RootVariableSlot* CreateVariable(const ShaderVariable& variable);
			DX11ShaderConstantBlock* CreateConstantBlock(const ShaderVariable& variable);

		private:
			DX11Device* m_Device;
			std::vector<DX11ShaderConstantBlock*> m_ConstantBlocks;
			std::vector<ID3D11SamplerState*> m_StaticSampler;
			std::vector<IDX11RootVariableSlot*> m_VariableSlots;
		};
	}
}

#endif