#pragma once

#include "..\Graphics\IShaderCompiler.h"
#include "DXCommon.h"
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
			DXShaderCompiler(DXShaderCompiler&& other) = delete;
			DXShaderCompiler& operator=(DXShaderCompiler&& other) = delete;

		public:
			DXShaderCompiler(IFactory* pFactory, bool debug);
			~DXShaderCompiler();
		
			ShaderByteCode CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info, std::string& errorString) const override final;
			ShaderByteCode CompileFromString(const std::string& src, const ShaderCompileInfo& info, std::string& errorString) const override final;
			
			IFactory* GetFactory() const override final;

		private:
			std::string GetShaderModel(SHADER_TYPE type) const;

		private:
			IFactory* m_Factory;
			int32 m_Flags;
		};
	}
}