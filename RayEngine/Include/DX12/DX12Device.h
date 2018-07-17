#include "..\Graphics\IDevice.h"
#include "DX12Common.h"

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

			bool CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const override final;
			bool CreateFence(IFence** fence) const override final;
			bool CreateShader(IShader** shader, const ShaderByteCode& byteCode) const override final;
			bool CreateRenderTargetView(IRenderTargetView** view, const RenderTargetViewInfo& info) const override final;
			bool CreateDepthStencilView(IDepthStencilView** view, const DepthStencilViewInfo& info) const override final;
			bool CreateTexture(ITexture** texture, const TextureInfo& info) const override final;

			DX12Device& operator=(DX12Device&& other);

		private:
			void Create(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer);
			DX12DescriptorHeap CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num,
				D3D12_DESCRIPTOR_HEAP_FLAGS flags);

		private:
			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			mutable DX12DescriptorHeap m_ResourceHeap;
			mutable DX12DescriptorHeap m_DsvHeap;
			mutable DX12DescriptorHeap m_RtvHeap;
		};
	}
}

#endif