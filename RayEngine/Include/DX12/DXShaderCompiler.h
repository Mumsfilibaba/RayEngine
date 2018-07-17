#pragma once

#include "..\Graphics\IShaderCompiler.h"
#include "DX12Common.h"
#include <d3dcompiler.h>

namespace RayEngine
{
	namespace Graphics
	{
		class DXShaderCompiler : public IShaderCompiler
		{
		public:
			DXShaderCompiler(const DXShaderCompiler& other) = delete;
			DXShaderCompiler& operator=(const DXShaderCompiler& other) = delete;

		public:
			DXShaderCompiler(bool debug);
			DXShaderCompiler(DXShaderCompiler&& other);
			DXShaderCompiler& operator=(DXShaderCompiler&& other);
			~DXShaderCompiler();
		
			ShaderByteCode CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info) const override final;
			ShaderByteCode CompileFromString(const std::string& src, const ShaderCompileInfo& info) const override final;

		private:
			std::string GetShaderModel(SHADERTYPE type) const;

		private:
			int32 m_Flags;
		};
	}
}