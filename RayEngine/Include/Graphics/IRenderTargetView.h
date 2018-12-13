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
#include <Interfaces/IObject.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IResource;


		/*///////////////////////////////////////////////////////////
			
			Struct that contains info of a RenderTargetView. Allows
			a resource to be used as a rendertarget.

			Name - Name of the view.

			Format - Format of the view

			ViewDimension - What kind of resource that is using
			the view. E.g is this a texture1D or texture2D

			pResource - The resource the is using the view

			The following variables used based on the value of 
			ViewDimension. E.g if the value is VIEWDIMENSION_BUFFER
			then only the buffer-struct should contain values.

			ElementCount - Number of elements in the buffer to
			use.

			FirstElement - Number of bytes to the first element 
			that the view should use.

			MipSlice - The first miplevel that the view should use.

			ArraySlice - The number of elements in the array that 
			the view can access. The first element is specified in
			FirstArraySlice.

			FirstArraySlice - The first element in the earray that 
			the view can access. 

			PlaneSlice - Some formats, e.g FORMAT_D24_UNORM_S8_UINT
			has multiple planes. One for depth and one for stencil.
			Specify 0 for the main plane (in this case depth). This
			means that for formats with only one plane e.g 
			FORMAT_R8G8B8A8_UNORM only the 0-plane is available.

		///////////////////////////////////////////////////////////*/
		struct RAYENGINE_API RenderTargetViewDesc
		{
			FORMAT Format;
			VIEWDIMENSION ViewDimension;
			IResource* pResource;

			union
			{
				struct
				{
					uint32 ElementCount;
					uint32 StartElement;
				} Buffer;

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
					uint32 PlaneSlice;
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipSlice;
					uint32 ArraySize;
					uint32 FirstArraySlice;
					uint32 PlaneSlice;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize;
					uint32 FirstArraySlice;
				} Texture2DMSArray;

				struct
				{
					uint32 MipSlice;
					uint32 DepthSliceCount;
					uint32 FirstDepthSlice;
				} Texture3D;
			};
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API IRenderTargetView : public IObject
		{
			RE_INTERFACE(IRenderTargetView);

		public:
			IRenderTargetView() {}
			~IRenderTargetView() {}

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(RenderTargetViewDesc* pDesc) const = 0;
		};
	}
}