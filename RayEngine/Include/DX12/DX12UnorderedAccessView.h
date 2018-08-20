#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IUnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12View.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12UnorderedAccessView final : public IUnorderedAccessView, public DX12View
		{
		public:
			DX12UnorderedAccessView(const DX12UnorderedAccessView& other) = delete;
			DX12UnorderedAccessView& operator=(const DX12UnorderedAccessView& other) = delete;
			DX12UnorderedAccessView(DX12UnorderedAccessView&& other) = delete;
			DX12UnorderedAccessView& operator=(DX12UnorderedAccessView&& other) = delete;

		public:
			DX12UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info);
			~DX12UnorderedAccessView();

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const UnorderedAccessViewInfo& info);

		private:
			DX12Device* m_Device;
		};
	}
}

#endif