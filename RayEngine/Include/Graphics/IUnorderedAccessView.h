#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IDevice;
		class IResource;



		/////////////////////////////////////////////////////////////
		enum UNORDERED_ACCESS_VIEW_FLAG : int32
		{
			UNORDERED_ACCESS_VIEW_FLAG_NONE = 0,
			UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW = (1 << 0),
			UNORDERED_ACCESS_VIEW_FLAG_BUFFER_COUNTER = (1 << 1),
		};



		/////////////////////////////////////////////////////////////
		struct UnorderedAccessViewInfo
		{
			std::string Name = "";
			int32 Flags = UNORDERED_ACCESS_VIEW_FLAG_NONE;
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			IResource* pResource = nullptr;
			IResource* pCounterResource = nullptr;

			union
			{
				struct
				{
					uint32 StartElement = 0;
					uint32 ElementCount = 0;
				} Buffer;

				struct
				{
					uint32 MipSlice = 0;
				} Texture1D;

				struct
				{
					uint32 MipSlice = 0;
					uint32 FirstArraySlice = 0;
					uint32 ArraySize = 0;
				} Texture1DArray;

				struct
				{
					uint32 MipSlice = 0;
					uint32 PlaneSlice = 0;
				} Texture2D;

				struct
				{
					uint32 MipSlice = 0;
					uint32 FirstArraySlice = 0;
					uint32 ArraySize = 0;
					uint32 PlaneSlice = 0;
				} Texture2DArray;

				struct
				{
					uint32 MipSlice = 0;
					uint32 DepthSliceCount = 0;
					uint32 FirstDepthSlice = 0;
				} Texture3D;
			};
		};



		/////////////////////////////////////////////////////////////
		class IUnorderedAccessView : public IDeviceObject
		{
		public:
			IUnorderedAccessView(IUnorderedAccessView&& other) = delete;
			IUnorderedAccessView(const IUnorderedAccessView& other) = delete;
			IUnorderedAccessView& operator=(IUnorderedAccessView&& other) = delete;
			IUnorderedAccessView& operator=(const IUnorderedAccessView& other) = delete;

		public:
			IUnorderedAccessView() {}
			virtual ~IUnorderedAccessView() {}
		};
	}
}