#pragma once

#include "..\Graphics\IDevice.h"
#include "DX12DescriptorHeap.h"
#include "DX12DynamicUploadHeap.h"
#include "DX12CommandQueue.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Device : public IDevice
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
			DX12DynamicUploadHeap* GetDX12UploadHeap() const;

			bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) override final;
			bool CreateShader(IShader** ppShader, const ShaderByteCode& byteCode) override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;
			System::Log* GetDeviceLog() override final;
			
			IFactory* GetFactory() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer);

		private:
			IFactory* m_Factory;

			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			DX12DynamicUploadHeap* m_UploadHeap;
			DX12CommandQueue* m_UploadQueue;
			DX12DescriptorHeap* m_ResourceHeap;
			DX12DescriptorHeap* m_DsvHeap;
			DX12DescriptorHeap* m_RtvHeap;

			mutable System::Log m_Log;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif