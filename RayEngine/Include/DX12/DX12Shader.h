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
			DX12Shader(ID3D12Device* device, const ShaderByteCode& byteCode);
			~DX12Shader();

			SHADERTYPE GetType() const override final;

		private:
			SHADERTYPE m_Type;
			D3D12_SHADER_BYTECODE m_Shader;
		};
	}
}