#pragma once

#include "..\Graphics\IRootVariableSlot.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class IDX12RootVariableSlot : public IRootVariableSlot
		{
		public:
			IDX12RootVariableSlot(const IDX12RootVariableSlot& other) = delete;
			IDX12RootVariableSlot& operator=(const IDX12RootVariableSlot& other) = delete;
			IDX12RootVariableSlot(IDX12RootVariableSlot&& other) = delete;
			IDX12RootVariableSlot& operator=(IDX12RootVariableSlot&& other) = delete;

		public:
			IDX12RootVariableSlot() {}
			virtual ~IDX12RootVariableSlot() {}

			virtual void SetShaderResourceViews(ID3D12GraphicsCommandList* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const = 0;
			virtual void SetSamplers(ID3D12GraphicsCommandList* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const = 0;
			virtual void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const = 0;
			virtual void SetConstantBuffers(ID3D12GraphicsCommandList* pContext, ID3D11Buffer** ppBuffers, int32 count) const = 0;
		};
	}
}

#endif