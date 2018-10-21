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
#include "..\Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Factory.h"

#define QueryDX11Device(pDevice) reinterpret_cast<DX11Device*>(pDevice->QueryReference())

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDeviceContext;
		class DX11DeviceContext;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device final : public IDevice
		{
			RE_IMPLEMENT_INTERFACE(DX11Device);

		public:
			DX11Device(IFactory* pFactory, const DeviceDesc& info, bool debugLayer);
			~DX11Device();

			ID3D11Device* GetD3D11Device() const;

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderDesc& info) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc& info) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc& info) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc& info) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc& info) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerDesc& info) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc& info) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc& info) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc& info) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc& info) override final;
		
			void SetName(const std::string& name) override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

			System::Log* GetDeviceLog() override final;


		private:
			void Create(IFactory* pFactory, const DeviceDesc& info, bool debugLayer);

		private:
			DX11Factory* mFactory;
			DX11DeviceContext* mImmediateContext;
			IDXGIAdapter* m_Adapter;
			ID3D11Device* m_Device;
			ID3D11Debug* m_DebugDevice;
			
			D3D_FEATURE_LEVEL m_FeatureLevel;
			
			System::Log mLog;

			IObject::CounterType mReferences;
			IObject::CounterType m_InternalReferences;
		};
	}
}

#endif