#pragma once

#include "..\Graphics\IDevice.h"
#include "DX12DescriptorHeap.h"
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

		public:
			DX12Device(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer);
			DX12Device(DX12Device&& other);
			~DX12Device();

			bool CreateCommandQueue(ICommandQueue** commandQueue, const CommandQueueInfo& info) const override final;
			bool CreateShader(IShader** shader, const ShaderByteCode& byteCode) const override final;
			bool CreateRenderTargetView(IRenderTargetView** view, const RenderTargetViewInfo& info) const override final;
			bool CreateDepthStencilView(IDepthStencilView** view, const DepthStencilViewInfo& info) const override final;
			bool CreateTexture(ITexture** texture, const ResourceData* const pInitialData, const TextureInfo& info) const override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) const override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const override final;
			System::Log* GetDeviceLog() const override final;

			DX12Device& operator=(DX12Device&& other);

			ID3D12Device* GetD3D12Device() const;
			const DX12CommandQueue* GetDX12CommandQueue() const;
			const DX12DescriptorHeap* GetDX12DepthStencilViewHeap() const;
			const DX12DescriptorHeap* GetDX12RenderTargetViewHeap() const;
			const DX12DescriptorHeap* GetDX12ResourceHeap() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer);

		private:
			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			DX12CommandQueue m_UploadQueue;
			mutable DX12DescriptorHeap m_ResourceHeap;
			mutable DX12DescriptorHeap m_DsvHeap;
			mutable DX12DescriptorHeap m_RtvHeap;
			mutable System::Log m_Log;
			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif