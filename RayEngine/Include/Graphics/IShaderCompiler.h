#pragma once

#include "IShader.h"
#include <string>

//namespace RayEngine
//{
//	namespace Graphics
//	{	/////////////////////////////////////////////////////////////
//		class IFactory;
//
//
//
//		/////////////////////////////////////////////////////////////
//		struct ShaderCompileInfo
//		{
//			std::string EntryPoint = "main";
//			SHADER_TYPE Type = SHADER_TYPE_UNKNOWN;
//			SHADER_SOURCE_LANG SrcLang = SHADER_SOURCE_LANG_UNKNOWN;
//		};
//
//
//
//
//		/////////////////////////////////////////////////////////////
//		class IShaderCompiler : public RefCounter
//		{
//		public:
//			virtual ~IShaderCompiler() {}
//
//			//Compiles shader source code from a file
//			virtual ShaderByteCode CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info, std::string& errorString) const = 0;
//			//Compiles shader source code from a string
//			virtual ShaderByteCode CompileFromString(const std::string& src, const ShaderCompileInfo& info, std::string& errorString) const = 0;
//			//Retrives the Factory that created the compiler
//			virtual IFactory* GetFactory() const = 0;
//		};
//	}
//}