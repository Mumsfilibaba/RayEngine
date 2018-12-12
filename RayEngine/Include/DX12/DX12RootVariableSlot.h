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
#include "RayEngine.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12RootVariableSlot
		{
			RE_UNIQUE_OBJECT(DX12RootVariableSlot);

		public:
			DX12RootVariableSlot(D3D12_RESOURCE_STATES neededState);
			virtual ~DX12RootVariableSlot();

			virtual void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const = 0;
			
			virtual void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const = 0;
			
			virtual void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const = 0;
			
			virtual void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const = 0;
			
			inline D3D12_RESOURCE_STATES GetNeededD3D12ResourceState() const
			{
				return m_NeededState;
			}

		private:
			D3D12_RESOURCE_STATES m_NeededState;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12GraphicsDescriptorRootSlot final : public DX12RootVariableSlot
		{
			RE_UNIQUE_OBJECT(DX12GraphicsDescriptorRootSlot);

		public:
			DX12GraphicsDescriptorRootSlot(D3D12_RESOURCE_STATES neededState, int32 baseSlot);
			~DX12GraphicsDescriptorRootSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_BaseSlot;
		};
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12ComputeDescriptorRootSlot final : public DX12RootVariableSlot
		{
			RE_UNIQUE_OBJECT(DX12ComputeDescriptorRootSlot);

		public:
			DX12ComputeDescriptorRootSlot(D3D12_RESOURCE_STATES neededState, int32 baseSlot);
			~DX12ComputeDescriptorRootSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_BaseSlot;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12GraphicsRootSignatureSlot final : public DX12RootVariableSlot
		{
			RE_UNIQUE_OBJECT(DX12GraphicsRootSignatureSlot);

		public:
			DX12GraphicsRootSignatureSlot(D3D12_RESOURCE_STATES neededState, int32 slot);
			~DX12GraphicsRootSignatureSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_Slot;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12ComputeRootSignatureSlot final : public DX12RootVariableSlot
		{
			RE_UNIQUE_OBJECT(DX12ComputeRootSignatureSlot);

		public:
			DX12ComputeRootSignatureSlot(D3D12_RESOURCE_STATES neededState, int32 slot);
			~DX12ComputeRootSignatureSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_Slot;
		};
	}
}

#endif