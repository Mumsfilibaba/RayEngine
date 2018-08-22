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
		enum SHADER_FLAGS : int32
		{
			SHADER_FLAGS_NONE = 0,
			SHADER_FLAGS_DEBUG = (1 << 0),
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