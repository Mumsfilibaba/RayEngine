#pragma once

#include "ISampler.h"

#if defined(max)
#undef max
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;
		class IBuffer;
		class ITexture;



		/////////////////////////////////////////////////////////////
		enum SHADER_SOURCE_LANG : int32
		{
			SHADER_SOURCE_LANG_UNKNOWN = 0,
			SHADER_SOURCE_LANG_GLSL = 1,
			SHADER_SOURCE_LANG_HLSL = 2,
		};



		/////////////////////////////////////////////////////////////
		enum VARIABLE_TYPE : int32
		{
			VARIABLE_TYPE_UNKNOWN = 0,
			VARIABLE_TYPE_UNIFORMBUFFER = 1,
			VARIABLE_TYPE_TEXTURE = 2,
			VARIABLE_TYPE_SAMPLER = 3,
			VARIABLE_TYPE_SHADER_CONSTANTS = 4
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_USAGE : int32
		{
			SHADER_USAGE_UNKNOWN = 0,
			SHADER_USAGE_DYNAMIC = 1,
			SHADER_USAGE_STATIC = 2,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_FLAGS : int32
		{
			SHADER_FLAGS_NONE = 0,
			SHADER_FLAGS_DEBUG = 1,
		};



		////////////////////////////////////////////////////////////
		struct ShaderInfo
		{
			std::string Name = "";
			std::string Source = "";
			std::string FilePath = "";
			std::string EntryPoint = "main";
			SHADER_TYPE Type = SHADER_TYPE_UNKNOWN;
			SHADER_SOURCE_LANG SrcLang = SHADER_SOURCE_LANG_UNKNOWN;
			int32 Flags =
#if defined(RE_DEBUG)
				SHADER_FLAGS_DEBUG;
#else
				SHADER_FLAGS_NONE;
#endif
		};

		

		/////////////////////////////////////////////////////////////
		class IShader : public IDeviceObject
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
		};
	}
}