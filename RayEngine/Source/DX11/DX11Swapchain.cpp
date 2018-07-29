#include "..\..\Include\DX11\DX11Swapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\DX11\DX11Factory.h"
#include "..\..\Include\DX11\DX11CommandQueue.h"
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Swapchain::DX11Swapchain(IFactory* pFactory, const SwapchainInfo& info)
			: m_Device(nullptr),
			m_Factory(nullptr),
			m_CommandQueue(nullptr),
			m_Swapchain(nullptr),
			m_BufferCount(0),
			m_CurrentBuffer(0),
			m_ReferenceCount(0),
			m_Texture()
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory);

			Create(pFactory, info);
		}



		/////////////////////////////////////////////////////////////
		DX11Swapchain::DX11Swapchain(DX11Swapchain&& other)
			: m_Device(other.m_Device),
			m_Factory(other.m_Factory),
			m_CommandQueue(other.m_CommandQueue),
			m_Swapchain(other.m_Swapchain),
			m_BufferCount(other.m_BufferCount),
			m_CurrentBuffer(other.m_CurrentBuffer),
			m_ReferenceCount(other.m_ReferenceCount),
			m_Texture(std::move(other.m_Texture))
		{
			other.m_Device = nullptr;
			other.m_Factory = nullptr;
			other.m_CommandQueue = nullptr;
			other.m_Swapchain = nullptr;
			other.m_BufferCount = 0;
			other.m_CurrentBuffer = 0;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX11Swapchain::~DX11Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
			
			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}

			if (m_CommandQueue != nullptr)
			{
				m_CommandQueue->Release();
				m_CommandQueue = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX11Swapchain& DX11Swapchain::operator=(DX11Swapchain&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Swapchain);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}

				if (m_Factory != nullptr)
				{
					m_Factory->Release();
					m_Factory = nullptr;
				}

				if (m_CommandQueue != nullptr)
				{
					m_CommandQueue->Release();
					m_CommandQueue = nullptr;
				}


				m_Device = other.m_Device;
				m_Factory = other.m_Factory;
				m_CommandQueue = other.m_CommandQueue;
				m_Swapchain = other.m_Swapchain;
				m_BufferCount = other.m_BufferCount;
				m_CurrentBuffer = other.m_CurrentBuffer;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_Factory = nullptr;
				other.m_CommandQueue = nullptr;
				other.m_Swapchain = nullptr;
				other.m_BufferCount = 0;
				other.m_CurrentBuffer = 0;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		int32 DX11Swapchain::GetCurrentBuffer() const
		{
			return m_CurrentBuffer;
		}



		/////////////////////////////////////////////////////////////
		ITexture* DX11Swapchain::GetBuffer(int32 index)
		{
			return &m_Texture;
		}



		/////////////////////////////////////////////////////////////
		const ITexture* DX11Swapchain::GetBuffer(int32 index) const
		{
			return &m_Texture;
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);
			
			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_BufferCount;
		}



		/////////////////////////////////////////////////////////////
		IFactory* DX11Swapchain::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		ICommandQueue* DX11Swapchain::GetCommandQueue() const
		{
			return m_CommandQueue;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11Swapchain::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Swapchain::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Swapchain::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::Create(IFactory* pFactory, const SwapchainInfo& info)
		{
			using namespace System;

			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = info.Buffer.Count;
			desc.BufferDesc.Format = ReToDXFormat(info.Buffer.Format);
			desc.BufferDesc.Height = info.Buffer.Height;
			desc.BufferDesc.Width = info.Buffer.Width;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.RefreshRate.Numerator = 0;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

			//TODO: Tearing?
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Windowed = true;
			
			HWND hWnd = reinterpret_cast<const Win32WindowImpl*>(info.pWindow->GetImplementation())->GetHWND();
			desc.OutputWindow = hWnd;

			m_CommandQueue = reinterpret_cast<ICommandQueue*>(info.pCommandQueue->QueryReference());
			m_Device = reinterpret_cast<IDevice*>(m_CommandQueue->GetDevice()->QueryReference());
			IDXGIFactory* pDXGIFactory = reinterpret_cast<DX11Factory*>(pFactory)->GetDXGIFactory();
			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(m_Device)->GetD3D11Device();
			
			HRESULT hr = pDXGIFactory->CreateSwapChain(pD3D11Device, &desc, &m_Swapchain);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create SwapChain. " + DXErrorString(hr));
				return;
			}

			m_BufferCount = info.Buffer.Count;
			CreateTextures();
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::CreateTextures()
		{
			using namespace Microsoft::WRL;
			using namespace System;

			ComPtr<ID3D11Texture2D> pTexture = nullptr;
			HRESULT hr = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&pTexture));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not get backbuffer" + DXErrorString(hr));
				return;
			}

			m_Texture = DX11Texture(m_Device, pTexture.Get());
		}
	}
}

#endif