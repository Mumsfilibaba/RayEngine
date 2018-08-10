#pragma once

#include "..\Graphics\IDevice.h"
#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Factory.h"

#define QueryDX11Device(pDevice) reinterpret_cast<DX11Device*>(pDevice->QueryReference())

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Device final : public IDevice
		{
		public:
			DX11Device(const DX11Device& other) = delete;
			DX11Device& operator=(const DX11Device& other) = delete;
			DX11Device(DX11Device&& other) = delete;
			DX11Device& operator=(DX11Device&& other) = delete;

		public:
			DX11Device(IFactory* pFactory, const DeviceInfo& info, bool debugLayer);
			~DX11Device();

			ID3D11Device* GetD3D11Device() const;
			ID3D11DeviceContext* GetD3D11DeviceContext() const;

			bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) override final;
			bool CreateShader(IShader** ppShader, const ShaderInfo& info) override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;
			void QueryFactory(IFactory** ppFactory) const override final;
			System::Log* GetDeviceLog() override final;

		private:
			void Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer);

		private:
			DX11Factory* m_Factory;
			IDXGIAdapter* m_Adapter;
			ID3D11Device* m_Device;
			ID3D11DeviceContext* m_ImmediateContext;
			ID3D11Debug* m_DebugDevice;
			D3D_FEATURE_LEVEL m_FeatureLevel;
			System::Log m_Log;
		};
	}
}

#endif