#pragma once

#include "..\RefCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
		enum SHADER_TYPE : int32
		{
			SHADER_TYPE_UNKNOWN = 0,
			SHADER_TYPE_VERTEX = 1,
			SHADER_TYPE_HULL = 2,
			SHADER_TYPE_DOMAIN = 3,
			SHADER_TYPE_GEOMETRY = 4,
			SHADER_TYPE_PIXEL = 5,
			SHADER_TYPE_COMPUTE = 6,
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
			ShaderByteCode(const std::string& entryPoint, SHADER_TYPE type, SHADER_SOURCE_LANG srcLang, int8* pBytes, int32 size);
			ShaderByteCode(const ShaderByteCode& other);
			ShaderByteCode(ShaderByteCode&& other);
			~ShaderByteCode();

			bool IsValid() const;
			int8* Detach();

			const std::string& GetEntryPoint() const;
			int32 GetSize() const;
			const int8* GetBytes() const;
			SHADER_TYPE GetType() const;
			SHADER_SOURCE_LANG GetSourceLanguage() const;

			ShaderByteCode& operator=(const ShaderByteCode& other);
			ShaderByteCode& operator=(ShaderByteCode&& other);

		private:
			void Release();

		private:
			SHADER_SOURCE_LANG m_Lang;
			SHADER_TYPE m_Type;
			int8* m_Bytes;
			int32 m_Size;
			std::string m_EntryPoint;
		};

		

		/////////////////////////////////////////////////////////////
		class IShader : public RefCounter
		{
		public:
			IShader(IShader&& other) = delete;
			IShader(const IShader& other) = delete;
			IShader& operator=(IShader&& other) = delete;
			IShader& operator=(const IShader& other) = delete;

		public:
			IShader() {}
			virtual ~IShader() {}

			//Retrives the shadertype
			virtual SHADER_TYPE GetType() const = 0;
			//Retrives the device that created the shader
			virtual IDevice* GetDevice() const = 0;
		};
	}
}