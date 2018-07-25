#pragma once

#include "..\IReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum SHADERTYPE : int32
		{
			SHADERTYPE_UNKNOWN = 0,
			SHADERTYPE_VERTEX = 1,
			SHADERTYPE_HULL = 2,
			SHADERTYPE_DOMAIN = 3,
			SHADERTYPE_GEOMETRY = 4,
			SHADERTYPE_PIXEL = 5,
			SHADERTYPE_COMPUTE = 6,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_SOURCE_LANG : int32
		{
			SHADER_SOURCE_LANG_UNKNOWN = 0,
			SHADER_SOURCE_LANG_GLSL = 1,
			SHADER_SOURCE_LANG_HLSL = 2,
		};



		/////////////////////////////////////////////////////////////
		class ShaderByteCode
		{
		public:
			ShaderByteCode();
			ShaderByteCode(SHADERTYPE type, SHADER_SOURCE_LANG srcLang, int8* pBytes, int32 size);
			ShaderByteCode(const ShaderByteCode& other);
			ShaderByteCode(ShaderByteCode&& other);
			~ShaderByteCode();

			int8* Detach();

			SHADER_SOURCE_LANG GetSourceLanguage() const;
			SHADERTYPE GetType() const;
			const int8* GetBytes() const;
			int32 GetSize() const;

			ShaderByteCode& operator=(const ShaderByteCode& other);
			ShaderByteCode& operator=(ShaderByteCode&& other);

		private:
			void Release();

		private:
			SHADER_SOURCE_LANG m_Lang;
			SHADERTYPE m_Type;
			int8* m_Bytes;
			int32 m_Size;
		};

		

		/////////////////////////////////////////////////////////////
		class IShader : public IReferenceCounter
		{
		public:
			IShader(IShader&& other) = delete;
			IShader(const IShader& other) = delete;
			IShader& operator=(IShader&& other) = delete;
			IShader& operator=(const IShader& other) = delete;

		public:
			IShader() {}
			virtual ~IShader() {}

			virtual SHADERTYPE GetType() const = 0;
		};
	}
}