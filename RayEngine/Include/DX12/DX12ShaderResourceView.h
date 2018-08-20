#pragma once

#include "..\Graphics\IDevice.h"
#include "..\Graphics\IShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12View.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12ShaderResourceView final : public IShaderResourceView, public DX12View
		{
		public:
			DX12ShaderResourceView(const DX12ShaderResourceView& other) = delete;
			DX12ShaderResourceView& operator=(const DX12ShaderResourceView& other) = delete;
			DX12ShaderResourceView(DX12ShaderResourceView&& other) = delete;
			DX12ShaderResourceView& operator=(DX12ShaderResourceView&& other) = delete;

		public:
			DX12ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info);
			~DX12ShaderResourceView();

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ShaderResourceViewInfo& info);

		private:
			DX12Device* m_Device;
		};
	}
}

#endif