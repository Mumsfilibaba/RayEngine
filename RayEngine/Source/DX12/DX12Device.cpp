#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Fence.h"
#include "..\..\Include\DX12\DX12Shader.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer)
			: m_Device(nullptr),
			m_Adapter(nullptr),
			m_DebugDevice(nullptr)
		{
			Create(factory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(DX12Device&& other)
			: m_Device(other.m_Device),
			m_Adapter(other.m_Adapter),
			m_DebugDevice(other.m_DebugDevice)
		{
			other.m_Device = nullptr;
			other.m_Adapter = nullptr;
			other.m_DebugDevice = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12Device::~DX12Device()
		{
			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL);
				D3DRelease(m_DebugDevice);
			}

			D3DRelease_S(m_Device);
			D3DRelease_S(m_Adapter);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const
		{
			return ((*commandQueue = new DX12CommandQueue(m_Device, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateFence(IFence** fence) const
		{
			return ((*fence = new DX12Fence(m_Device)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateShader(IShader** shader, const ShaderByteCode& byteCode) const
		{
			return ((*shader = new DX12Shader(m_Device, byteCode)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		DX12Device& DX12Device::operator=(DX12Device&& other)
		{
			if (this != &other)
			{
				m_Device = other.m_Device;
				m_Adapter = other.m_Adapter;
				m_DebugDevice = other.m_DebugDevice;

				other.m_Adapter = nullptr;
				other.m_Device = nullptr;
				other.m_DebugDevice = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Device::Create(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer)
		{
			if (SUCCEEDED(factory->EnumAdapters1(info.Adapter->ApiID, &m_Adapter)))
			{
				if (SUCCEEDED(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device))))
				{
					if (debugLayer)
					{
						if (SUCCEEDED(m_Device->QueryInterface<ID3D12DebugDevice>(&m_DebugDevice)))
							return;
					}
				}
			}
		}
	}
}

#endif