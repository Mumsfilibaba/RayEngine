/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include <string>
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IResource;



		/////////////////////////////////////////////////////////////
		enum SHADER_RESOURCE_VIEW_FLAG : int32
		{
			SHADER_RESOURCE_VIEW_FLAG_NONE = 0,
			SHADER_RESOURCE_VIEW_FLAG_RAW_BUFFER = (1 << 0),
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct ShaderResourceViewInfo
		{
			std::string Name = "";
			int32 Flags = SHADER_RESOURCE_VIEW_FLAG_NONE;
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			IResource* pResource = nullptr;

			union
			{
				struct
				{
					uint32 ElementCount;
					uint32 StartElement;
					uint32 ElementStrideBytes;
				} Buffer;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					float MinLODClamp;
				} Texture1D;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					uint32 ArraySize;
					uint32 FirstArraySlice;
					float MinLODClamp;
				} Texture1DArray;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					uint32 PlaneSlice;
					float MinLODClamp;
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					uint32 ArraySize;
					uint32 FirstArraySlice;
					uint32 PlaneSlice;
					float MinLODClamp;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize;
					uint32 FirstArraySlice;
				} Texture2DMSArray;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					float MinLODClamp;
				} Texture3D;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					float MinLODClamp;
				} TextureCube;

				struct
				{
					uint32 MipLevels;
					uint32 MostDetailedMip;
					uint32 CubeCount;
					uint32 First2DArrayFace;
					float MinLODClamp;
				} TextureCubeArray;
			};
		};



		/////////////////////////////////////////////////////////////
		class IShaderResourceView : public IDeviceObject
		{
			RE_INTERFACE(IShaderResourceView);

		public:
			IShaderResourceView() {}
			~IShaderResourceView() {}
		};
	}
}