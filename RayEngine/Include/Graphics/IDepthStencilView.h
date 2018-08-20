#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IResource;



		/////////////////////////////////////////////////////////////
		struct DepthStencilViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			const IResource* pResource = nullptr;

			union
			{
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
				} Texture2DMS;

				struct
				{
				} Texture2DArray;

				struct
				{
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