#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Shader.h"
#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Buffer.h"
#include "..\..\Include\DX12\DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
			: m_Factory(nullptr),
			m_Device(nullptr),
			m_Adapter(nullptr),
			m_DebugDevice(nullptr),
			m_ResourceHeap(),
			m_DsvHeap(),
			m_RtvHeap(),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory);
			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(DX12Device&& other)
			: m_Factory(other.m_Factory),
			m_Device(other.m_Device),
			m_Adapter(other.m_Adapter),
			m_DebugDevice(other.m_DebugDevice),
			m_ResourceHeap(std::move(other.m_ResourceHeap)),
			m_DsvHeap(std::move(other.m_DsvHeap)),
			m_RtvHeap(std::move(other.m_RtvHeap)),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Factory = nullptr;
			other.m_Device = nullptr;
			other.m_Adapter = nullptr;
			other.m_DebugDevice = nullptr;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12Device::~DX12Device()
		{
			D3DRelease_S(m_Device);
			D3DRelease_S(m_Adapter);

			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL);
				D3DRelease(m_DebugDevice);
			}

			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info)
		{
			return ((*ppCommandQueue = new DX12CommandQueue(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateShader(IShader** ppShader, const ShaderByteCode& byteCode)
		{
			return ((*ppShader = new DX12Shader(this, byteCode)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return ((*ppView = new DX12RenderTargetView(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return (*ppView = new DX12DepthStencilView(this, info));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return ((*ppTexture = new DX12Texture(this, pInitialData, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return ((*ppBuffer = new DX12Buffer(this, pInitialData, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info)
		{
			return ((*ppRootSignature = new DX12RootSignature(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return ((*ppPipelineState = new DX12PipelineState(this, info)));
		}



		/////////////////////////////////////////////////////////////
		System::Log* DX12Device::GetDeviceLog()
		{
			return &m_Log;
		}



		/////////////////////////////////////////////////////////////
		IFactory* DX12Device::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		DX12Device& DX12Device::operator=(DX12Device&& other)
		{
			if (this != &other)
			{
				if (m_Factory != nullptr)
				{
					m_Factory->Release();
					m_Factory = nullptr;
				}


				m_Factory = other.m_Factory;
				m_Device = other.m_Device;
				m_Adapter = other.m_Adapter;
				m_DebugDevice = other.m_DebugDevice;
				m_ResourceHeap = std::move(other.m_ResourceHeap);
				m_DsvHeap = std::move(other.m_DsvHeap);
				m_RtvHeap = std::move(other.m_RtvHeap);
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Factory = nullptr;
				other.m_Device = nullptr;
				other.m_Adapter = nullptr;
				other.m_DebugDevice = nullptr;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D12Device* DX12Device::GetD3D12Device() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		const DX12CommandQueue* DX12Device::GetDX12CommandQueue() const
		{
			return &m_UploadQueue;
		}



		/////////////////////////////////////////////////////////////
		const DX12DescriptorHeap* DX12Device::GetDX12DepthStencilViewHeap() const
		{
			return &m_DsvHeap;
		}



		/////////////////////////////////////////////////////////////
		const DX12DescriptorHeap* DX12Device::GetDX12RenderTargetViewHeap() const
		{
			return &m_RtvHeap;
		}



		/////////////////////////////////////////////////////////////
		const DX12DescriptorHeap* DX12Device::GetDX12ResourceHeap() const
		{
			return &m_ResourceHeap;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12Device::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Device::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Device::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////s
		uint32 DX12Device::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Device::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
			IDXGIFactory5* pDXGIFactory = reinterpret_cast<DX12Factory*>(pFactory)->GetDXGIFactory();

			if (SUCCEEDED(pDXGIFactory->EnumAdapters1(info.pAdapter->ApiID, &m_Adapter)))
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


					D3D12SetName(m_Device, info.Name);

					m_DsvHeap = DX12DescriptorHeap(this, info.Name + ": DSV-Heap", D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 2, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_RtvHeap = DX12DescriptorHeap(this, info.Name + ": RTV-Heap", D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 10, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_ResourceHeap = DX12DescriptorHeap(this, info.Name + ": Resource-Heap (CBV/SRV)", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 20, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);


					CommandQueueInfo queueInfo = {};
					queueInfo.Name = "Device UploadQueue";
					m_UploadQueue = DX12CommandQueue(this, queueInfo);
				}
			}
		}
	}
}

#endif