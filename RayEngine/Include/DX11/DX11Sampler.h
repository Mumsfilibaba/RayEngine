#pragma once

#include "..\Graphics\ISampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Sampler final : public ISampler
		{
		public:
			DX11Sampler(const DX11Sampler& other) = delete;
			DX11Sampler& operator=(const DX11Sampler& other) = delete;
			DX11Sampler(DX11Sampler&& other) = delete;
			DX11Sampler& operator=(DX11Sampler&& other) = delete;

		public:
			DX11Sampler(IDevice* pDevice, const SamplerInfo& info);
			~DX11Sampler();

			ID3D11SamplerState* GetD3D11SamplerState() const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const SamplerInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11SamplerState* m_SamplerState;
		};
	}
}

#endif