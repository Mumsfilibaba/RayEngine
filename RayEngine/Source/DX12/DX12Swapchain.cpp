#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(IDXGIFactory5* factory, const SwapchainInfo& info)
			: m_Swapchain(nullptr),
			m_BufferCount(0),
			m_CurrentBuffer(0),
			m_Textures()
		{
			Create(factory, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(DX12Swapchain&& other)
			: m_Swapchain(other.m_Swapchain)
		{
			other.m_Swapchain = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain::~DX12Swapchain()
		{
			D3DRelease_S(m_Swapchain);
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
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain& DX12Swapchain::operator=(DX12Swapchain&& other)
		{
			if (this != &other)
			{
				m_Swapchain = other.m_Swapchain;

				other.m_Swapchain = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Create(IDXGIFactory5* factory, const SwapchainInfo& info)
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

			//COMMANQUEUE??
			HWND hWnd = reinterpret_cast<const System::Win32WindowImpl*>(info.Window->GetImplementation())->GetHWND();
			ID3D12CommandQueue* queue = reinterpret_cast<DX12CommandQueue*>(info.commandQueue)->GetCommandQueue();
			
			if (FAILED(factory->CreateSwapChainForHwnd(queue, hWnd, &desc, nullptr, nullptr, &m_Swapchain)))
				return;


			factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);


			CreateTextures();
			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateTextures()
		{
			using namespace Microsoft::WRL;

			ComPtr<ID3D12Resource> buffer;
			for (int32 i = 0; i < m_BufferCount; i++)
			{
				if (SUCCEEDED(m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer))))
					m_Textures.push_back(DX12Texture(buffer.Get()));
			}
		}
	}
}