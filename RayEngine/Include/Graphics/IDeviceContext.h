/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include "IDeviceObject.h"
#include "..\Math\Rectangle.h"

namespace RayEngine
{
	namespace Graphics
	{	
		/////////////////////////////////////////////////////////////
		class ITexture;
		class IBuffer;
		class ISampler;
		class IRenderTargetView;
		class IDepthStencilView;
		class IShaderResourceView;
		class IUnorderedAccessView;
		class IRootLayout;
		class IPipelineState;

		struct Viewport;



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
			~IDeviceContext() {}

			/*////////////////////////////////////////////////////////////

				Clears a RendertargetView.

				pView - A valid pointer to an IRenderTargetView 
				interface.

				pColor - A four element float array containg the 
				clearcolor

			////////////////////////////////////////////////////////////*/
			virtual void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const = 0;
			

			/*////////////////////////////////////////////////////////////

				Clears a DepthStencilView.

				pView - A valid pointer to an IDepthStencilView
				interface.

				depth - A float containing the depthvalue to be set.

				stencil - An 8-bit unsigned int containging the stencil 
				value to be set.

			////////////////////////////////////////////////////////////*/
			virtual void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const = 0;


			/*////////////////////////////////////////////////////////////

				Sets RenderTargets and DepthStencil to be used in a 
				draw- or dispatchcall.

				pRenderTarget - A pointer to an IRenderTargetView
				interface. Can be nullptr if no RenderTarget is desired.

				pDepthStencil - A pointer to an IDepthStencilView
				interface. Can be nullptr if no DepthStencil is desired.

			////////////////////////////////////////////////////////////*/
			virtual void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets ShaderResources to be used in a draw- or 
				dispatchcall.

				pShaderResourceView - A pointer to an IShaderResourceView
				interface. Can be nullptr if no ShaderResource is desired
				or to unbind the current.

				startRootIndex - Index of the shadervariable in the 
				current RootLayout.

			////////////////////////////////////////////////////////////*/
			virtual void SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets UnorderedAccessViews to be used in a draw- or
				dispatchcall.

				pUnorderedAccessView - A pointer to an IUnorderedAccessView
				interface. Can be nullptr if no UnorderedAccessView is 
				desired or to unbind the current.

				startRootIndex - Index of the shadervariable in the
				current RootLayout.

			////////////////////////////////////////////////////////////*/
			virtual void SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const = 0;


			/*////////////////////////////////////////////////////////////

				Sets ConstantBuffers to be used in a draw- or
				dispatchcall.

				pBuffer - A pointer to an IBuffer interface. Can be 
				nullptr if no ConstantBuffer is desired or to unbind the 
				current.

				startRootIndex - Index of the shadervariable in the
				current RootLayout.

			////////////////////////////////////////////////////////////*/
			virtual void SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets Sampler to be used in a draw- or dispatchcall.

				pSampler - A pointer to an ISampler interface. Can be
				nullptr if no Sampler is desired or to unbind the
				current.

				startRootIndex - Index of the shadervariable in the
				current RootLayout.

			////////////////////////////////////////////////////////////*/
			virtual void SetSamplers(ISampler* pSampler, int32 startRootIndex) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets the current state of the pipeline to be used in a 
				draw- or dispatchcall.

				pPipelineState - A pointer to an IPipelineState interface.
				Can be nullptr to unbind the current.

			////////////////////////////////////////////////////////////*/
			virtual void SetPipelineState(IPipelineState* pPipelineState) const = 0;


			/*////////////////////////////////////////////////////////////

				Sets the RootLayout of the pipeline to be used in a
				draw- or dispatchcall.

				pRootLayout - A pointer to an IRootLayout interface.
				Can be nullptr to unbind the current.

			////////////////////////////////////////////////////////////*/
			virtual void SetRootLayout(IRootLayout* pRootLayout) const = 0;


			/*////////////////////////////////////////////////////////////

				Sets the buffers to load veretexdata from in a drawcall.

				pBuffer - A pointer to an IBuffer interface. Can be 
				nullptr to unbind the current.

				startSlot - Vertexslot to start binding to. 

			////////////////////////////////////////////////////////////*/
			virtual void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets the viewport to be used in a drawcall.

				viewport - A Viewport-structure defining the area to 
				render to.

			////////////////////////////////////////////////////////////*/
			virtual void SetViewports(const Viewport& viewport) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Sets the primitive type to be used in a drawcall.

				viewport - A Viewport-structure defining the area to
				render to.

			////////////////////////////////////////////////////////////*/
			virtual void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const = 0;


			/*////////////////////////////////////////////////////////////

				Sets a scissior rectangle to be used when rasterizing.
				All pixels outside the scissorrectangle will be discarded.

				rect - A Rectangle-structure defining the area to keep.

			////////////////////////////////////////////////////////////*/
			virtual void SetScissorRects(const Math::Rectangle& rect) const = 0;


			/*////////////////////////////////////////////////////////////

				Dispatches a drawcall.

				startVertex - Index in the current vertexbuffer to start
				at.

				vertexCount - Number of vertices to draw.

			////////////////////////////////////////////////////////////*/
			virtual void Draw(int32 startVertex, int32 vertexCount) const = 0;


			/*////////////////////////////////////////////////////////////

				Dispatches a drawcall with indexbuffer.

				startVertex - Index in the current vertexbuffer to start
				at.

				startIndex - Index in the current indexbuffer to start
				at.

				indexCount - Number of indices to draw.

			////////////////////////////////////////////////////////////*/
			virtual void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Dispatches a drawcall instanced. Requires atleast two 
				vertexbuffers. One containing the geometry and one 
				containing data to update each instance with.

				startVertex - Index in the current vertexbuffer to start
				at.

				vertexCount - Number of vertices to draw for each 
				instance.

				startInstance - Index in the current instancebuffer to
				start at.

				instanceCount - Number of instances to draw.

			////////////////////////////////////////////////////////////*/
			virtual void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const = 0;


			/*////////////////////////////////////////////////////////////

				Dispatches a drawcall instanced using an indexbuffer. 
				Requires atleast two vertexbuffers. One containing the 
				geometry and one containing data to update each instance
				with.

				startVertex - Index in the current vertexbuffer to start
				at.

				startIndex - Index in the current indexbuffer to start
				at.

				indexCount - Number of indices to draw for each instance.

				startInstance - Index in the current instancebuffer to
				start at.

				instanceCount - Number of instances to draw.

			////////////////////////////////////////////////////////////*/
			virtual void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				A dispatch call starting a compute pipeline with ceratain
				amount of threadgroups.

				threadGroupCountX - Number of threadgroups in the X 
				direction.

				threadGroupCountY - Number of threadgroups in the Y
				direction.

				threadGroupCountZ - Number of threadgroups in the Z
				direction.

			////////////////////////////////////////////////////////////*/
			virtual void Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Executes a deffered context and resets the context state

				pDefferedContext - A valid pointer to an IDeviceContext
				interface to execute.

			////////////////////////////////////////////////////////////*/
			virtual void ExecuteDefferedContext(IDeviceContext* pDefferedContext) const = 0;

			//Flushes the commandqueue - Syncronizes the CPU and GPU
			virtual void Flush() const = 0;
			//Reset allocator and list
			virtual bool Reset() const = 0;
			//Close list for commandsubmission
			virtual bool Close() const = 0;
		};
	}
}