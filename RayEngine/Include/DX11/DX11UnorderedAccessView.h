#pragma once

#include "..\Graphics\IUnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11UnorderedAccessView final : public IUnorderedAccessView
		{
		public:
			DX11UnorderedAccessView(const DX11UnorderedAccessView& other) = delete;
			DX11UnorderedAccessView& operator=(const DX11UnorderedAccessView& other) = delete;
			DX11UnorderedAccessView(DX11UnorderedAccessView&& other) = delete;
			DX11UnorderedAccessView& operator=(DX11UnorderedAccessView&& other) = delete;

		public:
			DX11UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info);
			~DX11UnorderedAccessView();

			ID3D11UnorderedAccessView* GetD3D11UnorderedAccessView() const;
			
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const UnorderedAccessViewInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11UnorderedAccessView* m_View;
		};
	}
}

#endif