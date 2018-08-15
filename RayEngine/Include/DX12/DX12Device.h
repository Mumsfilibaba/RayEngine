#pragma once

#include "..\Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

#define QueryDX12Device(pDevice) reinterpret_cast<DX12Device*>(pDevice->QueryReference())

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Factory;
		class DX12DeviceContext;
		class DX12DescriptorHeap;
		class DX12DynamicUploadHeap;



		/////////////////////////////////////////////////////////////
		class DX12Device final : public IDevice
		{
		public:
			DX12Device(const DX12Device& other) = delete;
			DX12Device& operator=(const DX12Device& other) = delete;
			DX12Device(DX12Device&& other) = delete;
			DX12Device& operator=(DX12Device&& other) = delete;

		public:
			DX12Device(IFactory* pFactory, const DeviceInfo& info, bool debugLayer);
			~DX12Device();

			ID3D12Device* GetD3D12Device() const;
			DX12CommandQueue* GetDX12CommandQueue() const;
			DX12DescriptorHeap* GetDX12DepthStencilViewHeap() const;
			DX12DescriptorHeap* GetDX12RenderTargetViewHeap() const;
			DX12DescriptorHeap* GetDX12ResourceHeap() const;
			DX12DescriptorHeap* GetDX12SamplerHeap() const;
			DX12DynamicUploadHeap* GetDX12UploadHeap() const;

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			bool CreateShader(IShader** ppShader, const ShaderInfo& info) override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutInfo& info) override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;
			void QueryFactory(IFactory** ppFactory) const override final;
			
			System::Log* GetDeviceLog() override final;

		private:
			void Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer);

		private:
			DX12Factory* m_Factory;
			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			DX12DeviceContext* m_ImmediateContext;
			DX12DynamicUploadHeap* m_UploadHeap;
			DX12CommandQueue* m_UploadQueue;
			DX12DescriptorHeap* m_ResourceHeap;
			DX12DescriptorHeap* m_DsvHeap;
			DX12DescriptorHeap* m_RtvHeap;
			DX12DescriptorHeap* m_SamplerHeap;
			mutable System::Log m_Log;
		};
	}
}

#endif