#pragma once

#include "IShader.h"

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