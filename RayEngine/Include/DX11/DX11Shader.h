#pragma once

#include "..\Graphics\IShader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Shader : public IShader
		{ 
		public:
			DX11Shader(const DX11Shader& other) = delete;
			DX11Shader& operator=(const DX11Shader& other) = delete;
			DX11Shader(DX11Shader&& other) = delete;
			DX11Shader& operator=(DX11Shader&& other) = delete;

		public:
			DX11Shader(IDevice* pDevice, const ShaderByteCode& byteCode);
			~DX11Shader();

			const ShaderByteCode& GetByteCode() const;
			ID3D11VertexShader* GetD3D11VertexShader() const;
			ID3D11HullShader* GetD3D11HullShader() const;
			ID3D11DomainShader* GetD3D11DomainShader() const;
			ID3D11GeometryShader* GetD3D11GeometryShader() const;
			ID3D11PixelShader* GetD3D11PixelShader() const;
			ID3D11ComputeShader* GetD3D11ComputeShader() const;

			SHADER_TYPE GetType() const override final;
			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const ShaderByteCode& byteCode);

		private:
			IDevice* m_Device;

			ShaderByteCode m_ByteCode;
			SHADER_TYPE m_Type;
			union
			{
				ID3D11VertexShader* m_VertexShader;
				ID3D11HullShader* m_HullShader;
				ID3D11DomainShader* m_DomainShader;
				ID3D11GeometryShader* m_GeometryShader;
				ID3D11PixelShader* m_PixelShader;
				ID3D11ComputeShader* m_ComputeShader;
			};

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif