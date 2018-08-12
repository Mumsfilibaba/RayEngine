#include "..\..\Include\DX11\DX11DeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11DeviceContext::DX11DeviceContext(IDevice* pDevice, bool isDeffered)
			: m_Device(nullptr),
			m_Context(nullptr),
			m_CommandList(nullptr),
			m_IsDeffered(false)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(isDeffered);
		}



		/////////////////////////////////////////////////////////////
		DX11DeviceContext::~DX11DeviceContext()
		{
			D3DRelease_S(m_Context);
			D3DRelease_S(m_CommandList);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetViewports(const Viewport& viewport) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::TransitionResource(ITexture* resource, RESOURCE_STATE to, int32 subresource) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Flush() const
		{
		}



		/////////////////////////////////////////////////////////////
		bool DX11DeviceContext::Reset() const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool DX11DeviceContext::Close() const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Create(bool isDeffered)
		{
		}
	}
}

#endif