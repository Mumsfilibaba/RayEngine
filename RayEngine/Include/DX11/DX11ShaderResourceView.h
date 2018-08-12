#pragma once

#include "..\Graphics\IShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11ShaderResourceView final : public IShaderResourceView
		{
		public:
			DX11ShaderResourceView(const DX11ShaderResourceView& other) = delete;
			DX11ShaderResourceView& operator=(const DX11ShaderResourceView& other) = delete;
			DX11ShaderResourceView(DX11ShaderResourceView&& other) = delete;
			DX11ShaderResourceView& operator=(DX11ShaderResourceView&& other) = delete;

		public:
			DX11ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info);
			~DX11ShaderResourceView();

			ID3D11ShaderResourceView* GetD3D11ShaderResourceView() const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ShaderResourceViewInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11ShaderResourceView* m_View;
		};
	}
}

#endif