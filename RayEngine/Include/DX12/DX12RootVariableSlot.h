#pragma once

#include "..\Graphics\IRootVariableSlot.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
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

			virtual void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const = 0;
			virtual void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const = 0;
			virtual void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const = 0;
			virtual void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const = 0;
		};



		/////////////////////////////////////////////////////////////
		class DX12GraphicsDescriptorRootSlot final : public IDX12RootVariableSlot
		{
		public:
			DX12GraphicsDescriptorRootSlot(const DX12GraphicsDescriptorRootSlot& other) = delete;
			DX12GraphicsDescriptorRootSlot& operator=(const DX12GraphicsDescriptorRootSlot& other) = delete;
			DX12GraphicsDescriptorRootSlot(DX12GraphicsDescriptorRootSlot&& other) = delete;
			DX12GraphicsDescriptorRootSlot& operator=(DX12GraphicsDescriptorRootSlot&& other) = delete;

		public:
			DX12GraphicsDescriptorRootSlot(int32 baseSlot);
			~DX12GraphicsDescriptorRootSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_BaseSlot;
		};
		


		/////////////////////////////////////////////////////////////
		class DX12ComputeDescriptorRootSlot final : public IDX12RootVariableSlot
		{
		public:
			DX12ComputeDescriptorRootSlot(const DX12ComputeDescriptorRootSlot& other) = delete;
			DX12ComputeDescriptorRootSlot& operator=(const DX12ComputeDescriptorRootSlot& other) = delete;
			DX12ComputeDescriptorRootSlot(DX12ComputeDescriptorRootSlot&& other) = delete;
			DX12ComputeDescriptorRootSlot& operator=(DX12ComputeDescriptorRootSlot&& other) = delete;

		public:
			DX12ComputeDescriptorRootSlot(int32 baseSlot);
			~DX12ComputeDescriptorRootSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_BaseSlot;
		};



		/////////////////////////////////////////////////////////////
		class DX12GraphicsRootSignatureSlot final : public IDX12RootVariableSlot
		{
		public:
			DX12GraphicsRootSignatureSlot(const DX12GraphicsRootSignatureSlot& other) = delete;
			DX12GraphicsRootSignatureSlot& operator=(const DX12GraphicsRootSignatureSlot& other) = delete;
			DX12GraphicsRootSignatureSlot(DX12GraphicsRootSignatureSlot&& other) = delete;
			DX12GraphicsRootSignatureSlot& operator=(DX12GraphicsRootSignatureSlot&& other) = delete;

		public:
			DX12GraphicsRootSignatureSlot(int32 slot);
			~DX12GraphicsRootSignatureSlot();

			void SetShaderResourceViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* ppSRVs, int32 count) const override final;
			void SetSamplers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pSamplerStates, int32 count) const override final;
			void SetUnorderedAccessViews(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pUAVs, int32 count) const override final;
			void SetConstantBuffers(ID3D12GraphicsCommandList* pCommandList, DX12DescriptorHandle* pBuffers, int32 count) const override final;

		private:
			int32 m_Slot;
		};



		/////////////////////////////////////////////////////////////
		class DX12ComputeRootSignatureSlot final : public IDX12RootVariableSlot
		{
		public:
			DX12ComputeRootSignatureSlot(const DX12ComputeRootSignatureSlot& other) = delete;
			DX12ComputeRootSignatureSlot& operator=(const DX12ComputeRootSignatureSlot& other) = delete;
			DX12ComputeRootSignatureSlot(DX12ComputeRootSignatureSlot&& other) = delete;
			DX12ComputeRootSignatureSlot& operator=(DX12ComputeRootSignatureSlot&& other) = delete;

		public:
			DX12ComputeRootSignatureSlot(int32 slot);
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