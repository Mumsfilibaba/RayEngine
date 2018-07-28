#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Swapchain.h"
#include "..\..\Include\DX12\DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(IFactory* pFactory, const SwapchainInfo& info)
			: m_Factory(nullptr),
			m_CommandQueue(nullptr),
			m_Swapchain(nullptr),
			m_BufferCount(0),
			m_CurrentBuffer(0),
			m_Textures(),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory->QueryReference());

			Create(pFactory, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(DX12Swapchain&& other)
			: m_Factory(other.m_Factory),
			m_CommandQueue(other.m_CommandQueue),
			m_Swapchain(other.m_Swapchain),
			m_BufferCount(other.m_BufferCount),
			m_CurrentBuffer(other.m_BufferCount),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Factory = nullptr;
			other.m_CommandQueue = nullptr;
			other.m_Swapchain = nullptr;
			other.m_BufferCount = 0;
			other.m_CurrentBuffer = 0;
			other.m_ReferenceCount = 0;

			other.m_Textures.swap(m_Textures);
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain::~DX12Swapchain()
		{
			D3DRelease_S(m_Swapchain);
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
		int32 DX12Swapchain::GetCurrentBuffer() const
		{
			return m_CurrentBuffer;
		}



		/////////////////////////////////////////////////////////////
		ITexture* DX12Swapchain::GetBuffer(int32 index)
		{
			DX12Texture* ptr = &m_Textures[index];
			return reinterpret_cast<ITexture*>(ptr);
		}



		/////////////////////////////////////////////////////////////
		const ITexture* DX12Swapchain::GetBuffer(int32 index) const
		{
			const DX12Texture* ptr = &m_Textures[index];
			return reinterpret_cast<const ITexture*>(ptr);
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);

			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_BufferCount;
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain& DX12Swapchain::operator=(DX12Swapchain&& other)
		{
			if (this != &other)
			{
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


				m_Factory = other.m_Factory;
				m_CommandQueue = other.m_CommandQueue;
				m_Swapchain = other.m_Swapchain;
				m_BufferCount = other.m_BufferCount;
				m_CurrentBuffer = other.m_CurrentBuffer;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Factory = nullptr;
				other.m_CommandQueue = nullptr;
				other.m_Swapchain = nullptr;
				other.m_BufferCount = 0;
				other.m_CurrentBuffer = 0;
				other.m_ReferenceCount = 0;

				other.m_Textures.swap(m_Textures);
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		IFactory* DX12Swapchain::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		ICommandQueue* DX12Swapchain::GetCommandQueue() const
		{
			return m_CommandQueue;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12Swapchain::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Swapchain::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Swapchain::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Create(IFactory* pFactory, const SwapchainInfo& info)
		{
			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.BufferCount = m_BufferCount = info.Buffer.Count;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Width = info.Buffer.Width;
			desc.Height = info.Buffer.Height;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SampleDesc.Count = 1;
			
			//TODO: Engine swap effect
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			
			//TODO: Tearing?
			desc.Flags = 0;

			//COMMANDQUEUE??
			HWND hWnd = reinterpret_cast<const System::Win32WindowImpl*>(info.pWindow->GetImplementation())->GetHWND();
			IDXGIFactory5* pDXGIFactory = reinterpret_cast<DX12Factory*>(pFactory)->GetDXGIFactory();

			m_CommandQueue = reinterpret_cast<ICommandQueue*>(info.pCommandQueue->QueryReference());
			ID3D12CommandQueue* pD3D12queue = reinterpret_cast<DX12CommandQueue*>(m_CommandQueue)->GetD3D12CommandQueue();
			

			if (FAILED(pDXGIFactory->CreateSwapChainForHwnd(pD3D12queue, hWnd, &desc, nullptr, nullptr, &m_Swapchain)))
				return;


			pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);


			CreateTextures();
			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateTextures()
		{
			using namespace Microsoft::WRL;

			ComPtr<ID3D12Resource> buffer = nullptr;
			for (int32 i = 0; i < m_BufferCount; i++)
			{
				if (SUCCEEDED(m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer))))
				{
					D3D12SetName(buffer.Get(), "Swapchain Buffer " + std::to_string(i));

					m_Textures.emplace_back(DX12Texture(m_CommandQueue->GetDevice(), buffer.Get()));
				}
			}

			m_Textures.shrink_to_fit();
		}
	}
}

#endif