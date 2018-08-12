#pragma once

#include "IDeviceObject.h"
#include "Viewport.h"
#include "..\Math\Rectangle.h"

namespace RayEngine
{
	namespace Graphics
	{	
		/////////////////////////////////////////////////////////////
		class ITexture;
		class IBuffer;
		class IRenderTargetView;
		class IDepthStencilView;
		class IShaderResourceView;
		class IUnorderedAccessView;
		class IRootLayout;
		class IPipelineState;



		/////////////////////////////////////////////////////////////
		class IDeviceContext : public IDeviceObject
		{
		public:
			IDeviceContext(IDeviceContext&& other) = delete;
			IDeviceContext(const IDeviceContext& other) = delete;
			IDeviceContext& operator=(IDeviceContext&& other) = delete;
			IDeviceContext& operator=(const IDeviceContext& other) = delete;

		public:
			IDeviceContext() {}
			virtual ~IDeviceContext() {}

			//Clears a rendertargetview
			virtual void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const = 0;
			//Clears a depthstncilview
			virtual void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const = 0;
			//Sets rendertargets- and depthstencilview
			virtual void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const = 0;
			//Sets an array of ShaderResourceViews
			virtual void SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const = 0;
			//Sets an array of UnorderedAccessView
			virtual void SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const = 0;
			//Sets an array of constantbuffers
			virtual void SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const = 0;
			//Sets a pipelinestate
			virtual void SetPipelineState(IPipelineState* pPipelineState) const = 0;
			//Sets a rootlayout
			virtual void SetRootLayout(IRootLayout* pRootLayout) const = 0;
			//Sets vertexbuffers to the pipeline
			virtual void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const = 0;
			//Sets the viewport of the screen
			virtual void SetViewports(const Viewport& viewport) const = 0;
			//Sets the primitive topology
			virtual void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const = 0;
			//Sets the scissorrect of the screen
			virtual void SetScissorRects(const Math::Rectangle& rect) const = 0;
			//Transition a resource from one state to another
			virtual void TransitionResource(ITexture* resource, RESOURCE_STATE to, int32 subresource) const = 0;
			//Draws the current vertexbuffer
			virtual void Draw(int32 startVertex, int32 vertexCount) const = 0;
			//Draws the current vertexbuffer indexed
			virtual void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const = 0;
			//Draws the current vertexbuffer instanced
			virtual void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const = 0;
			//Draws the current vertexbuffer instanced and indexed
			virtual void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const = 0;
			//Flushes the commandqueue - Syncronizes the CPU and GPU
			virtual void Flush() const = 0;
			//Reset allocator and list
			virtual bool Reset() const = 0;
			//Close list for commandsubmission
			virtual bool Close() const = 0;
			//Execure a DeviceContext that is created as deffered
			virtual void ExecuteDefferedContext(IDeviceContext* pDefferedContext) const = 0;
		};
	}
}