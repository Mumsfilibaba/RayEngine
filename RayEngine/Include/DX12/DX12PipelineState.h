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
#include "..\Graphics\IPipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Shader;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12PipelineState final : public IPipelineState
		{
			RE_IMPLEMENT_INTERFACE(DX12PipelineState);

		public:
			DX12PipelineState(IDevice* pdevice, const PipelineStateDesc* pDesc);
			~DX12PipelineState();

			inline ID3D12PipelineState* GetD3D12PipelineState() const
			{
				return m_PipelineState;
			}

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(PipelineStateDesc* pDesc) const;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const PipelineStateDesc* pDesc);

			void CreateGraphicsState();

			void CreateComputeState();
			
		private:
			static void SetShaderByteCode(D3D12_SHADER_BYTECODE* pByteCode, const DX12Shader* pShader);

			static void SetInputElementDesc(D3D12_INPUT_ELEMENT_DESC* pD3D12Desc, const InputElementDesc* pDesc);

			static void SetRasterizerDesc(D3D12_RASTERIZER_DESC* pD3D12Desc, const RasterizerStateDesc* pDesc);

			static void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC* pD3D12Desc, const DepthStencilStateDesc* pDesc);

			static void SetBlendDesc(D3D12_BLEND_DESC* pD3D12Desc, const BlendStateDesc* pDesc);

		private:
			DX12Device* m_Device;
			ID3D12PipelineState* m_PipelineState;
			
			PipelineStateDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}

#endif