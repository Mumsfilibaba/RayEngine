#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
		enum TEXTURE_TYPE : int32
		{
			TEXTURE_TYPE_UNKNOWN = 0,
			TEXTURE_TYPE_1D = 1,
			TEXTURE_TYPE_2D = 2,
			TEXTURE_TYPE_3D = 3,
		};



		/////////////////////////////////////////////////////////////
		enum TEXTURE_FLAGS : int32
		{
			TEXTURE_FLAGS_NONE = 0,
			TEXTURE_FLAGS_RENDERTARGET = (1 << 0),
			TEXTURE_FLAGS_SHADER_RESOURCE = (1 << 1),
			TEXTURE_FLAGS_DEPTH_STENCIL = (1 << 2),
			TEXTURE_FLAGS_UNORDERED_ACCESS = (1 << 2),
			TEXTURE_FLAGS_CUBEMAP = (1 << 3),
			TEXTURE_FLAGS_GENERATE_MIPLEVELS = (1 << 4),
		};



		/////////////////////////////////////////////////////////////
		struct TextureInfo
		{
			std::string Name = "";
			TEXTURE_TYPE Type = TEXTURE_TYPE_2D;
			FORMAT Format = FORMAT_UNKNOWN;
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			int32 Flags = TEXTURE_FLAGS_NONE;
			int32 CpuAccess = CPU_ACCESS_FLAG_NONE;
			int32 Width = 0;
			int32 Height = 0;
			int32 DepthOrArraySize = 0;
			int32 SampleCount = 1;
			int32 MipLevels = 1;			
			float OptimizedColor[4];
			
			struct
			{
				float OptimizedDepth = 1.0f;
				uint8 OptimizedStencil = 0;
			} DepthStencil;
		};



		/////////////////////////////////////////////////////////////
		class ITexture : public IDeviceObject
		{
		public:
			ITexture(const ITexture& other) = delete;
			ITexture(ITexture&& other) = delete;
			ITexture& operator=(const ITexture& other) = delete;
			ITexture& operator=(ITexture&& other) = delete;

		public:
			ITexture() {}
			virtual ~ITexture() {}

			//Returns the current state of the resource
			virtual RESOURCE_STATE GetResourceState() const = 0;
		};
	}
}