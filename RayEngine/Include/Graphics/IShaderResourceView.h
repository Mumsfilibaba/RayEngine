#pragma once

#include <string>
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IResource;



		/////////////////////////////////////////////////////////////
		struct ShaderResourceViewInfo
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
				} TextureCube;

				struct
				{
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
			virtual ~IShaderResourceView() {}
		};
	}
}