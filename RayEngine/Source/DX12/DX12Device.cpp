#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Shader.h"
#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Buffer.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer)
			: m_Device(nullptr),
			m_Adapter(nullptr),
			m_DebugDevice(nullptr),
			m_ResourceHeap(),
			m_DsvHeap(),
			m_RtvHeap()
		{
			Create(factory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(DX12Device&& other)
			: m_Device(other.m_Device),
			m_Adapter(other.m_Adapter),
			m_DebugDevice(other.m_DebugDevice),
			m_ResourceHeap(std::move(other.m_ResourceHeap)),
			m_DsvHeap(std::move(other.m_DsvHeap)),
			m_RtvHeap(std::move(other.m_RtvHeap))
		{
			other.m_Device = nullptr;
			other.m_Adapter = nullptr;
			other.m_DebugDevice = nullptr;
			memset(&other.m_ResourceHeap, 0, sizeof(other.m_ResourceHeap));
			memset(&other.m_DsvHeap, 0, sizeof(other.m_DsvHeap));
			memset(&other.m_RtvHeap, 0, sizeof(other.m_RtvHeap));
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
		bool DX12Device::CreateShader(IShader** shader, const ShaderByteCode& byteCode) const
		{
			return ((*shader = new DX12Shader(m_Device, byteCode)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRenderTargetView(IRenderTargetView** view, const RenderTargetViewInfo& info) const
		{
			return ((*view = new DX12RenderTargetView(m_Device, m_RtvHeap, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateDepthStencilView(IDepthStencilView** view, const DepthStencilViewInfo& info) const
		{
			return (*view = new DX12DepthStencilView(m_Device, m_DsvHeap, info));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateTexture(ITexture** texture, const TextureInfo& info) const
		{
			return ((*texture = new DX12Texture(m_Device, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) const
		{
			return ((*ppBuffer = new DX12Buffer(m_Device, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const
		{
			return ((*ppRootSignature = new DX12RootSignature(m_Device, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const
		{
			return ((*ppPipelineState = new DX12PipelineState(m_Device, info)));
		}



		/////////////////////////////////////////////////////////////
		DX12Device& DX12Device::operator=(DX12Device&& other)
		{
			if (this != &other)
			{
				m_Device = other.m_Device;
				m_Adapter = other.m_Adapter;
				m_DebugDevice = other.m_DebugDevice;
				m_ResourceHeap = std::move(other.m_ResourceHeap);
				m_DsvHeap = std::move(other.m_DsvHeap);
				m_RtvHeap = std::move(other.m_RtvHeap);

				other.m_Device = nullptr;
				other.m_Adapter = nullptr;
				other.m_DebugDevice = nullptr;
				memset(&other.m_ResourceHeap, 0, sizeof(other.m_ResourceHeap));
				memset(&other.m_DsvHeap, 0, sizeof(other.m_DsvHeap));
				memset(&other.m_RtvHeap, 0, sizeof(other.m_RtvHeap));
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Device::Create(IDXGIFactory5* factory, const DeviceInfo& info, bool debugLayer)
		{
			if (SUCCEEDED(factory->EnumAdapters1(info.pAdapter->ApiID, &m_Adapter)))
			{
				if (SUCCEEDED(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device))))
				{
					if (debugLayer)
					{
						if (FAILED(m_Device->QueryInterface<ID3D12DebugDevice>(&m_DebugDevice)))
						{
							return;
						}
					}


					m_DsvHeap = DX12DescriptorHeap(m_Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 2, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_RtvHeap = DX12DescriptorHeap(m_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 10, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_ResourceHeap = DX12DescriptorHeap(m_Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 20, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
				}
			}
		}
	}
}

#endif