#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{	/////////////////////////////////////////////////////////////
		class IDevice;
		class ITexture;



		/////////////////////////////////////////////////////////////
		struct UnorderedAccessViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			ITexture* pResource = nullptr;
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