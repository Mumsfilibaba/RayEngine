#pragma once

#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{	
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
				} Texture2DMS;

				struct
				{
				} Texture2DArray;

				struct
				{
				} Texture2DMSArray;

				struct
				{
				} Texture3D;

				struct
				{
				} Texture3DMS;

				struct
				{
				} Texture3DArray;

				struct
				{
				} Texture3DMSArray;
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