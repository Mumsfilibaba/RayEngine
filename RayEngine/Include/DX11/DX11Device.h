/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include "Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11DeviceContext;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device final : public IDevice
		{
			RE_IMPLEMENT_INTERFACE(DX11Device);

		public:
			DX11Device(const DeviceDesc* pDesc);
			~DX11Device();

			inline ID3D11Device* GetD3D11Device() const
			{
				return m_Device;
			}

			inline IDXGIFactory* GetDXGIFactory() const
			{
				return m_Factory;
			}

			inline IDXGIAdapter* GetDXGIAdapter() const
			{
				return m_Adapter;
			}

			IRenderer* CreateRenderer() override final;

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderDesc* pDesc) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc) override final;

			void GetDesc(DeviceDesc* pDesc) const override final;

			void GetAdapterDesc(AdapterDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const DeviceDesc* pDesc);

			bool QueryAdapter(D3D_FEATURE_LEVEL* pFeatureLevel);

		private:
			DX11DeviceContext* m_pImmediateContext;
			IDXGIFactory* m_Factory;
			IDXGIAdapter* m_Adapter;
			ID3D11Device* m_Device;
			ID3D11Debug* m_DebugDevice;
			
			D3D_FEATURE_LEVEL m_FeatureLevel;
			
			DeviceDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif