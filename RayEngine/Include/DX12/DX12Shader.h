#pragma once

#include "..\Graphics\IShader.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Shader : public IShader
		{
		public:
			DX12Shader(const DX12Shader& other) = delete;
			DX12Shader& operator=(const DX12Shader& other) = delete;

		public:
			DX12Shader(const ShaderByteCode& byteCode);
			~DX12Shader();

			SHADERTYPE GetType() const override final;

			const D3D12_SHADER_BYTECODE* GetD3D12ByteCode() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			D3D12_SHADER_BYTECODE m_Shader;
			SHADERTYPE m_Type;
			mutable uint32 m_ReferenceCount;
		};
	}
}