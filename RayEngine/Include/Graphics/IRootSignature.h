#pragma once

#include "IBuffer.h"
#include "ITexture.h"
#include "ISampler.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
		enum VIEW_TYPE : int32
		{
			VIEW_TYPE_UNKNOWN = 0,
			VIEW_TYPE_UNIFORMBUFFER = 1,
			VIEW_TYPE_TEXTURE = 2,
			VIEW_TYPE_SAMPLER = 3,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_VISIBILITY : int32
		{
			SHADER_VISIBILITY_UNKNOWN = 0,
			SHADER_VISIBILITY_VERTEX = 1,
			SHADER_VISIBILITY_HULL = 2,
			SHADER_VISIBILITY_DOMAIN = 3,
			SHADER_VISIBILITY_GEOMETRY = 4,
			SHADER_VISIBILITY_PIXEL = 5,
			SHADER_VISIBILITY_ALL = 6,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_USAGE : int32
		{
			SHADER_USAGE_UNKNOWN = 0,
			SHADER_USAGE_DYNAMIC = 1,
			SHADER_USAGE_STATIC = 2,
		};



		/////////////////////////////////////////////////////////////
		/*
		ShaderVariables defines a variable in the shader. This can be a texture 
		*/
		/////////////////////////////////////////////////////////////
		struct ShaderVariable
		{
			VIEW_TYPE ViewType = VIEW_TYPE_UNKNOWN;
			SHADER_USAGE ShaderUsage = SHADER_USAGE_UNKNOWN;
			SHADER_VISIBILITY ShaderVisibility = SHADER_VISIBILITY_UNKNOWN;
			int32 ShaderRegister = 0;

			union
			{
				IBuffer* pBuffer = nullptr;
				ITexture* pTexture;
				ISampler* pSampler;
			};
		};



		/////////////////////////////////////////////////////////////
		struct RootSignatureInfo
		{
			std::string Name = "";
			PIPELINE_TYPE PipelineType = PIPELINE_TYPE_UNKNOWN;
			int32 RootSignatureVisibility = SHADER_VISIBILITY_UNKNOWN;
			int32 ParameterCount = 0;
			ShaderVariable* pParameters = nullptr;
		};



		/////////////////////////////////////////////////////////////
		class IRootSignature : public RefCounter
		{
		public:
			IRootSignature(IRootSignature&& other) = delete;
			IRootSignature(const IRootSignature& other) = delete;
			IRootSignature& operator=(IRootSignature&& other) = delete;
			IRootSignature& operator=(const IRootSignature& other) = delete;

		public:
			IRootSignature() {}
			virtual ~IRootSignature() {}

			//Gets the device that created the rootsignature
			virtual IDevice* GetDevice() const = 0;
		};
	}
}