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
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class IResource;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum UNORDERED_ACCESS_VIEW_FLAG : int32
		{
			UNORDERED_ACCESS_VIEW_FLAG_NONE = 0,
			UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW = (1 << 0),
			UNORDERED_ACCESS_VIEW_FLAG_BUFFER_COUNTER = (1 << 1),
		};


		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct UnorderedAccessViewDesc
		{
			std::string Name;
			int32 Flags;
			FORMAT Format;
			VIEWDIMENSION ViewDimension;
			IResource* pResource;
			IResource* pCounterResource;

			union
			{
				struct
				{
					uint32 StartElement;
					uint32 ElementCount;
				} Buffer;

				struct
				{
					uint32 MipSlice;
				} Texture1D;

				struct
				{
					uint32 MipSlice;
					uint32 FirstArraySlice;
					uint32 ArraySize;
				} Texture1DArray;

				struct
				{
					uint32 MipSlice;
					uint32 PlaneSlice;
				} Texture2D;

				struct
				{
					uint32 MipSlice;
					uint32 FirstArraySlice;
					uint32 ArraySize;
					uint32 PlaneSlice;
				} Texture2DArray;

				struct
				{
					uint32 MipSlice;
					uint32 DepthSliceCount;
					uint32 FirstDepthSlice;
				} Texture3D;
			};
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IUnorderedAccessView : public IDeviceObject
		{
			RE_INTERFACE(IUnorderedAccessView);

		public:
			IUnorderedAccessView() {}
			~IUnorderedAccessView() {}

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(UnorderedAccessViewDesc* pDesc) const = 0;
		};
	}
}