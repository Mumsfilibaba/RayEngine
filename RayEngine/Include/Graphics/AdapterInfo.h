#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include <string>

namespace RayEngine
{
	namespace Graphics
	{
		enum ADAPTER_FLAGS : int32
		{
			ADAPTER_FLAGS_NONE = (1 << 0),
			ADAPTER_FLAGS_SOFTWAREADAPTER = (1 << 1),
			ADAPTER_FLAGS_GEOMETRYSHADER = (1 << 2),
			ADAPTER_FLAGS_TESSELATIONSHADER = (1 << 3),
			ADAPTER_FLAGS_SWAPCHAIN = (1 << 4),
			ADAPTER_FLAGS_COMPUTE = (1 << 5),
			ADAPTER_FLAGS_GRAPHICS = (1 << 6),
			ADAPTER_FLAGS_INTEGRATED = (1 << 7),
			ADAPTER_FLAGS_DISCRETE = (1 << 8),
			ADAPTER_FLAGS_VIRTUAL = (1 << 9),
			ADAPTER_FLAGS_CPU = (1 << 10),
		};

		struct AdapterInfo
		{
			std::string Vendor;
			std::string Model;
			int32 ID = 0;
			int64 Flags = ADAPTER_FLAGS_NONE;

			struct
			{
				struct
				{
					int32 Width = 0;
				} Texture1D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
				} Texture2D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
					int32 Depth = 0;
				} Texture3D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
				} TextureCube;

				int32 RenderTargetCount = 0;
			} Limits;
		};
	}
}