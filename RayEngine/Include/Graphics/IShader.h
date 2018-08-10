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



		/////////////////////////////////////////////////////////////
		enum STATIC_SAMPLER_BORDER_COLOR : int32
		{
			STATIC_SAMPLER_BORDER_COLOR_UNKNOWN = 0,
			STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK = 1,
			STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK = 2,
			STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE = 3,
		};



		/*////////////////////////////////////////////////////////////
			ShaderVariables defines a variable in the shader. This 
			can be a texture, buffer or sampler.

			VariableType - Type of variable

			ShaderUsage - If the variable will be updated between

			ShaderRegister - Register that the resource is mapped
			to

			ShaderSpace - ShaderSpace that the resource will be
			mapped to. This makes it posible to have different
			resources with the same registervalue, but different
			ShaderSpaces.
		
		////////////////////////////////////////////////////////////*/
		struct ShaderVariable
		{
			VARIABLE_TYPE Type = VARIABLE_TYPE_UNKNOWN;
			SHADER_USAGE ShaderUsage = SHADER_USAGE_UNKNOWN;
			int32 ShaderRegister = 0;
			int32 ShaderSpace = 0;

			union
			{
				IBuffer* pBuffer = nullptr;
				ITexture* pTexture;
				ISampler* pSampler;
				int32 ConstantCount;
			};
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct StaticSampler
		{
			SAMPLER_FILTER_MODE FilterMode = SAMPLER_FILTER_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressU = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressV = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressW = SAMPLER_ADRESS_MODE_UNKNOWN;
			float MipLODBias = 0.0f;
			float MinLOD = 0.0f;
			float MaxLOD = std::numeric_limits<float>::max();
			uint32 MaxAnistropy = 16;
			COMPARISON_FUNC ComparisonFunc = COMPARISON_FUNC_UNKNOWN;
			STATIC_SAMPLER_BORDER_COLOR BorderColor = STATIC_SAMPLER_BORDER_COLOR_UNKNOWN;
			int32 ShaderRegister = 0;
			int32 ShaderSpace = 0;
		};



		/*////////////////////////////////////////////////////////////
			Name - Name of the shaderobject
			
			Source - A string with the sourcecode that will be 
			compiled. If path is set then the Source is the 
			filename that contains the shadersource

			Filepath - Path to shader file

			EntryPoint - Name of the shaders 'main'-function

			Type - Type of shader

			SrcLang - SourceLanguage to be compiled

			pVariables - Array of ShaderVariables. If no 
			variables are used in the shader (Textures, 
			Buffers or Samplers) this can be zero. A
			StaticSampler does not count as a shadervariable

			VariableCount - The number of elements in pVariables

		////////////////////////////////////////////////////////////*/
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
			ShaderVariable* pVariables = nullptr;
			int32 VariableCount = 0;
			StaticSampler* pSamplers = nullptr;
			int32 SamplerCount = 0;
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