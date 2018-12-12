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
		class IResource;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum DEPTH_STENCIL_VIEW_FLAGS : int32
		{
			DEPTH_STENCIL_VIEW_FLAGS_NONE = 0,
			DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_DEPTH = (1 << 0),
			DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_STENCIL = (1 << 1)
		};


		/*///////////////////////////////////////////////////////////

			Struct that contains info of a DepthStencilView. Allows
			a resource to be used as a depth- and/or stencil-buffer.

			Name - Name of the view.

			Flags - Specify a DEPTH_STENCIL_VIEW_FLAGS-flag

			Format - Format of the view

			ViewDimension - What kind of resource that is using
			the view. E.g is this a texture1D or texture2D

			pResource - The resource the is using the view

			The following variables used based on the value of
			ViewDimension. E.g if the value is 
			VIEWDIMENSION_TEXTURE1D then only the texture1D-struct 
			should contain values.

			MipSlice - The first miplevel that the view should use.

			ArraySlice - The number of elements in the array that
			the view can access. The first element is specified in
			FirstArraySlice.

			FirstArraySlice - The first element in the earray that
			the view can access.

		///////////////////////////////////////////////////////////*/
		struct RAYENGINE_API DepthStencilViewDesc
		{
			int32 Flags;
			FORMAT Format;
			VIEWDIMENSION ViewDimension;
			IResource* pResource;

			union
			{
				struct
				{
					uint32 MipSlice;
				} Texture1D;

				struct
				{
					uint32 MipSlice;
					uint32 ArraySize;
					uint32 FirstArraySlice;
				} Texture1DArray;

				struct
				{
					uint32 MipSlice;
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipSlice;
					uint32 ArraySize;
					uint32 FirstArraySlice;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize;
					uint32 FirstArraySlice;
				} Texture2DMSArray;
			};
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API IDepthStencilView : public IDeviceObject
		{
			RE_INTERFACE(IDepthStencilView);

		public:
			IDepthStencilView() {}
			~IDepthStencilView() {}

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(DepthStencilViewDesc* pDesc) const = 0;
		};
	}
}