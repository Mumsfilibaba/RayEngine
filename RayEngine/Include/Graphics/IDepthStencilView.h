#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IResource;



		/////////////////////////////////////////////////////////////
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
		ViewDimension. E.g if the value is VIEWDIMENSION_TEXTURE1D
		then only the texture1D-struct should contain values.

		MipSlice - The first miplevel that the view should use.

		ArraySlice - The number of elements in the array that
		the view can access. The first element is specified in
		FirstArraySlice.

		FirstArraySlice - The first element in the earray that
		the view can access.

		///////////////////////////////////////////////////////////*/
		struct DepthStencilViewInfo
		{
			std::string Name = "";
			int32 Flags = DEPTH_STENCIL_VIEW_FLAGS_NONE;
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			IResource* pResource = nullptr;

			union
			{
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
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
					uint32 MipSlice = 0;
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
				} Texture2DArray;

				struct
				{
					uint32 ArraySize = 0;
					uint32 FirstArraySlice = 0;
				} Texture2DMSArray;
			};
		};



		/////////////////////////////////////////////////////////////
		class IDepthStencilView : public IDeviceObject
		{
		public:
			IDepthStencilView(IDepthStencilView&& other) = delete;
			IDepthStencilView(const IDepthStencilView& other) = delete;
			IDepthStencilView& operator=(IDepthStencilView&& other) = delete;
			IDepthStencilView& operator=(const IDepthStencilView& other) = delete;

		public:
			IDepthStencilView() {}
			virtual ~IDepthStencilView() {}
		};
	}
}