#pragma once

#include "DXCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <d3dcompiler.h>

namespace RayEngine
{
	namespace Graphics
	{
		class DXShaderBase
		{
		public:
			DXShaderBase(const DXShaderBase& other) = delete;
			DXShaderBase& operator=(const DXShaderBase& other) = delete;
			DXShaderBase(DXShaderBase&& other) = delete;
			DXShaderBase& operator=(DXShaderBase&& other) = delete;

		protected:
			DXShaderBase();
			~DXShaderBase();

			bool CompileFromFile(const std::string& filename, const std::string& filepath, const std::string& entryPoint, SHADER_TYPE type, int32 flags, std::string& errorString);
			bool CompileFromString(const std::string& src, const std::string& entryPoint, SHADER_TYPE type, int32 flags, std::string& errorString);

		public:
			ID3DBlob* GetBlob() const;

		protected:
			ID3DBlob* m_ShaderBlob;
			SHADER_TYPE m_Type;

		private:
			static std::string GetShaderModel(SHADER_TYPE shaderType);
		};
	}
}

#endif