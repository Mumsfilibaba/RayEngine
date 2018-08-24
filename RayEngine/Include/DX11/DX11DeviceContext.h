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

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

#define QueryDX11DeviceContext(pContext) reinterpret_cast<DX11DeviceContext*>(pContext->QueryReference());

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11RootLayout;
		class DX11PipelineState;



		/////////////////////////////////////////////////////////////
		class DX11DeviceContext final : public IDeviceContext
		{
		public:
			DX11DeviceContext(const DX11DeviceContext& other) = delete;
			DX11DeviceContext& operator=(const DX11DeviceContext& other) = delete;
			DX11DeviceContext(DX11DeviceContext&& other) = delete;
			DX11DeviceContext& operator=(DX11DeviceContext&& other) = delete;

		public:
			DX11DeviceContext(IDevice* pDevice, bool isDeffered);
			~DX11DeviceContext();

			ID3D11DeviceContext* GetD3D11DeviceContext() const;
			ID3D11CommandList* GetD3D11CommandList() const;

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

		private:
			void Create(bool isDeffered);
			void SetGraphicsPipeline(DX11PipelineState* pGraphicsPipeline) const;
			void SetComputePipeline(DX11PipelineState* pComputePipeline) const;

		private:
			DX11Device* m_Device;
			mutable DX11RootLayout* m_CurrentRootLayout;
			mutable DX11PipelineState* m_CurrentPipelineState;
			ID3D11DeviceContext* m_Context;
			mutable ID3D11CommandList* m_CommandList;
			bool m_IsDeffered;
		};
	}
}

#endif