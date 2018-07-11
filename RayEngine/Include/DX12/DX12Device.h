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

			DX12Device& operator=(DX12Device&& other);

		private:
			void Create(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer);

		private:
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			IDXGIAdapter1* m_Adapter;
		};
	}
}

#endif