#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IDevice;
		class IResource;



		/////////////////////////////////////////////////////////////
		struct UnorderedAccessViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			IResource* pResource = nullptr;

			union
			{
				struct
				{
				} Buffer;

				struct
				{
				} Texture1D;

				struct
				{
				} Texture1DArray;

				struct
				{
				} Texture2D;

				struct
				{
				} Texture2DArray;

				struct
				{
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