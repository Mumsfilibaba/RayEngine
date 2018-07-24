#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
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
			TEXTURE_FLAGS_RENDERTARGET = 1,
			TEXTURE_FLAGS_TEXTURE = 2,
			TEXTURE_FLAGS_DEPTHBUFFER = 3,
		};



		/////////////////////////////////////////////////////////////
		struct TextureInfo
		{
			std::string Name = "";
			TEXTURE_FLAGS Flags = TEXTURE_FLAGS_NONE;
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			CPU_ACCESS_FLAG CpuAccess = CPU_ACCESS_FLAG_NONE;
			TEXTURE_TYPE Type = TEXTURE_TYPE_2D;
			FORMAT Format = FORMAT_UNKNOWN;
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
		class ITexture
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