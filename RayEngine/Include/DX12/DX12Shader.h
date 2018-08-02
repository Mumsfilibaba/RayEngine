#pragma once

#include "..\Graphics\IShader.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Shader : public IShader
		{
		public:
			DX12Shader(const DX12Shader& other) = delete;
			DX12Shader& operator=(const DX12Shader& other) = delete;
			DX12Shader(DX12Shader&& other) = delete;
			DX12Shader& operator=(DX12Shader&& other) = delete;

		public:
			DX12Shader(IDevice* pDevice, const ShaderByteCode& byteCode);
			~DX12Shader();

			const D3D12_SHADER_BYTECODE* GetD3D12ByteCode() const;
			SHADER_TYPE GetType() const override final;
			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			IDevice* m_Device;
			
			D3D12_SHADER_BYTECODE m_Shader;
			SHADER_TYPE m_Type;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif