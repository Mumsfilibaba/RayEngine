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
#include "..\Graphics\IDeviceContext.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class GLDevice;
		class GLRootLayout;
		class GLPipelineState;



		/////////////////////////////////////////////////////////////
		class GLDeviceContext final : public IDeviceContext
		{
		public:
			GLDeviceContext(const GLDeviceContext& other) = delete;
			GLDeviceContext& operator=(const GLDeviceContext& other) = delete;
			GLDeviceContext(GLDeviceContext&& other) = delete;
			GLDeviceContext& operator=(GLDeviceContext&& other) = delete;

		public:
			GLDeviceContext(IDevice* pDevice, bool isDeffered);
			~GLDeviceContext();

			void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const override final;
			
			void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const override final;

			void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const override final;
			
			void SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const override final;
			
			void SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const override final;
			
			void SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const override final;
			
			void SetSamplers(ISampler* pSampler, int32 startRootIndex) const override final;
			
			void SetPipelineState(IPipelineState* pPipelineState) const override final;
			
			void SetRootLayout(IRootLayout* pRootLayout) const override final;
			
			void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const override final;
			
			void SetViewports(const Viewport& viewport) const override final;
			
			void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const override final;
			
			void SetScissorRects(const Math::Rectangle& rect) const override final;

			void Draw(int32 startVertex, int32 vertexCount) const override final;
			
			void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const override final;
			
			void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const override final;
			
			void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const override final;

			void Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const override final;

			void Flush() const override final;
			
			bool Reset() const override final;
			
			bool Close() const override final;

			void ExecuteDefferedContext(IDeviceContext* pDefferedContext) const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(bool isDeffered);

		private:
			GLDevice* m_Device;
			mutable GLRootLayout* m_CurrentRootLayout;
			mutable GLPipelineState* m_CurrentPipelineState;
			
			GLNativeContext m_Context;
			
			bool m_IsDeffered;
			
			IObject::CounterType m_References;
		};
	}
}