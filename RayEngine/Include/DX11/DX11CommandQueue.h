#pragma once

#include "..\..\Include\Graphics\ICommandQueue.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11CommandQueue final : public ICommandQueue
		{
		public:
			DX11CommandQueue(const DX11CommandQueue& other) = delete;
			DX11CommandQueue& operator=(const DX11CommandQueue& other) = delete;
			DX11CommandQueue(DX11CommandQueue&& other) = delete;
			DX11CommandQueue& operator=(DX11CommandQueue&& other) = delete;

		public:
			DX11CommandQueue(IDevice* pDevice, const CommandQueueInfo& info);
			~DX11CommandQueue();

			ID3D11CommandList* GetD3D11CommandList() const;
			ID3D11DeviceContext* GetD3D11DefferedContext() const;

			void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const override final;
			void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const override final;
			 
			void SetPipelineState(IPipelineState* pPipelineState) const override final;
			void SetRootSignature(IRootSignature* pRootSignature) const override final;
			void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const override final;

			//TODO: Multiple Viewports, rects and rendertargets
			void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const override final;
			void SetViewports(const Viewport& viewport) const override final;
			void SetScissorRects(const Math::Rectangle& rect) const override final;
			void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const override final;

			void TransitionResource(ITexture* pResource, RESOURCE_STATE to, int32 subresource) const override final;

			void Draw(int32 startVertex, int32 vertexCount) const override final;
			void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const override final;
			void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const override final;
			void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const override final;

			void Flush() const override final;
			void Execute() const override final;
			bool Reset() const override final;
			bool Close() const override final;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const CommandQueueInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11DeviceContext* m_DefferedContext;
			mutable ID3D11CommandList* m_List;
		};
	}
}

#endif