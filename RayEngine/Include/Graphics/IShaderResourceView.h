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
					uint32 ElementCount = 0;
					uint32 StartElement = 0;
					uint32 ElementStrideBytes = 0;
				} Buffer;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					float MinLODClamp = 0.0f;
				} Texture1D;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
					float MinLODClamp = 0.0f;
				} Texture1DArray;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					uint32 PlaneSlice = 0;
					float MinLODClamp = 0.0f;
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
					uint32 PlaneSlice = 0;
					float MinLODClamp = 0.0f;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
				} Texture2DMSArray;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					float MinLODClamp = 0.0f;
				} Texture3D;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					float MinLODClamp = 0.0f;
				} TextureCube;

				struct
				{
					uint32 MipLevels = 0;
					uint32 MostDetailedMip = 0;
					uint32 CubeCount = 0;
					uint32 First2DArrayFace = 0;
					float MinLODClamp = 0.0f;
				} TextureCubeArray;
			};
		};



		/////////////////////////////////////////////////////////////
		class IShaderResourceView : public IDeviceObject
		{
		public:
			IShaderResourceView(IShaderResourceView&& other) = delete;
			IShaderResourceView(const IShaderResourceView& other) = delete;
			IShaderResourceView& operator=(IShaderResourceView&& other) = delete;
			IShaderResourceView& operator=(const IShaderResourceView& other) = delete;

		public:
			IShaderResourceView() {}
			~IShaderResourceView() {}
		};
	}
}