#pragma once

#include <string>
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IDevice;
		class ITexture;



		/////////////////////////////////////////////////////////////
		struct ShaderResourceViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			ITexture* pResource = nullptr;
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