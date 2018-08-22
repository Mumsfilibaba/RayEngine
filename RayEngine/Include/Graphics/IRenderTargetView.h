#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
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
		struct RenderTargetViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			const IResource* pResource = nullptr;

			union
			{
				struct
				{
					uint32 ElementCount = 0;
					uint32 StartElement = 0;
				} Buffer;

				struct
				{
					uint32 MipSlice = 0;
				} Texture1D;

				struct
				{
					uint32 MipSlice = 0;
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
				} Texture1DArray;

				struct
				{
					uint32 MipSlice = 0;
					uint32 PlaneSlice = 0;
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipSlice = 0;
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
					uint32 PlaneSlice = 0;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
				} Texture2DMSArray;

				struct
				{
					uint32 MipSlice = 0;
					uint32 DepthSliceCount = 0;
					uint32 FirstDepthSlice = 0;
				} Texture3D;
			};
		};



		/////////////////////////////////////////////////////////////
		class IRenderTargetView : public IDeviceObject
		{
		public:
			IRenderTargetView(IRenderTargetView&& other) = delete;
			IRenderTargetView(const IRenderTargetView& other) = delete;
			IRenderTargetView& operator=(IRenderTargetView&& other) = delete;
			IRenderTargetView& operator=(const IRenderTargetView& other) = delete;

		public:
			IRenderTargetView() {}
			virtual ~IRenderTargetView() {}
		};
	}
}