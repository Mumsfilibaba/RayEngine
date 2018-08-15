#pragma once

#include "IShader.h"

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
		class ISampler;



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
			SHADER_TYPE ShaderStage = SHADER_TYPE_UNKNOWN;
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



		////////////////////////////////////////////////////////////
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



		/////////////////////////////////////////////////////////////
		struct RootLayoutInfo
		{
			std::string Name = "";
			PIPELINE_TYPE PipelineType = PIPELINE_TYPE_UNKNOWN;
			int32 RootSignatureVisibility = SHADER_VISIBILITY_UNKNOWN;
			ShaderVariable* pVariables = nullptr;
			int32 VariableCount = 0;
			StaticSampler* pStaticSamplers = nullptr;
			int32 SamplerCount = 0;
		};



		/////////////////////////////////////////////////////////////
		class IRootLayout : public IDeviceObject
		{
		public:
			IRootLayout(IRootLayout&& other) = delete;
			IRootLayout(const IRootLayout& other) = delete;
			IRootLayout& operator=(IRootLayout&& other) = delete;
			IRootLayout& operator=(const IRootLayout& other) = delete;

		public:
			IRootLayout() {}
			virtual ~IRootLayout() {}
		};
	}
}