#pragma once

#include "..\Graphics\IRootVariableSlot.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12RootVariableSlot : public IRootVariableSlot
		{
		public:
			DX12RootVariableSlot(const DX12RootVariableSlot& other) = delete;
			DX12RootVariableSlot& operator=(const DX12RootVariableSlot& other) = delete;
			DX12RootVariableSlot(DX12RootVariableSlot&& other) = delete;
			DX12RootVariableSlot& operator=(DX12RootVariableSlot&& other) = delete;

		public:
			DX12RootVariableSlot(D3D12_RESOURCE_STATES neededState);
			virtual ~DX12RootVariableSlot();

			virtual void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const = 0;
			virtual void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const = 0;
			virtual void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const = 0;
			virtual void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const = 0;
			
			D3D12_RESOURCE_STATES GetNeededD3D12ResourceState() const;

		private:
			D3D12_RESOURCE_STATES m_NeededState;
		};



		/////////////////////////////////////////////////////////////
		class DX12GraphicsDescriptorRootSlot final : public DX12RootVariableSlot
		{
		public:
			DX12GraphicsDescriptorRootSlot(const DX12GraphicsDescriptorRootSlot& other) = delete;
			DX12GraphicsDescriptorRootSlot& operator=(const DX12GraphicsDescriptorRootSlot& other) = delete;
			DX12GraphicsDescriptorRootSlot(DX12GraphicsDescriptorRootSlot&& other) = delete;
			DX12GraphicsDescriptorRootSlot& operator=(DX12GraphicsDescriptorRootSlot&& other) = delete;

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
		


		/////////////////////////////////////////////////////////////
		class DX12ComputeDescriptorRootSlot final : public DX12RootVariableSlot
		{
		public:
			DX12ComputeDescriptorRootSlot(const DX12ComputeDescriptorRootSlot& other) = delete;
			DX12ComputeDescriptorRootSlot& operator=(const DX12ComputeDescriptorRootSlot& other) = delete;
			DX12ComputeDescriptorRootSlot(DX12ComputeDescriptorRootSlot&& other) = delete;
			DX12ComputeDescriptorRootSlot& operator=(DX12ComputeDescriptorRootSlot&& other) = delete;

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



		/////////////////////////////////////////////////////////////
		class DX12GraphicsRootSignatureSlot final : public DX12RootVariableSlot
		{
		public:
			DX12GraphicsRootSignatureSlot(const DX12GraphicsRootSignatureSlot& other) = delete;
			DX12GraphicsRootSignatureSlot& operator=(const DX12GraphicsRootSignatureSlot& other) = delete;
			DX12GraphicsRootSignatureSlot(DX12GraphicsRootSignatureSlot&& other) = delete;
			DX12GraphicsRootSignatureSlot& operator=(DX12GraphicsRootSignatureSlot&& other) = delete;

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



		/////////////////////////////////////////////////////////////
		class DX12ComputeRootSignatureSlot final : public DX12RootVariableSlot
		{
		public:
			DX12ComputeRootSignatureSlot(const DX12ComputeRootSignatureSlot& other) = delete;
			DX12ComputeRootSignatureSlot& operator=(const DX12ComputeRootSignatureSlot& other) = delete;
			DX12ComputeRootSignatureSlot(DX12ComputeRootSignatureSlot&& other) = delete;
			DX12ComputeRootSignatureSlot& operator=(DX12ComputeRootSignatureSlot&& other) = delete;

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