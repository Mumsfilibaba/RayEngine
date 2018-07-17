#pragma once

#include "IShader.h"
#include <string>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct ShaderCompileInfo
		{
			std::string EntryPoint = "main";
			SHADERTYPE Type = SHADERTYPE_UNKNOWN;
			SHADER_SOURCE_LANG SrcLang = SHADER_SOURCE_LANG_UNKNOWN;
		};




		/////////////////////////////////////////////////////////////
		class IShaderCompiler
		{
		public:
			virtual ~IShaderCompiler() {}

			virtual ShaderByteCode CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info) const = 0;
			virtual ShaderByteCode CompileFromString(const std::string& src, const ShaderCompileInfo& info) const = 0;
		};
	}
}