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
			const ITexture* Resource = nullptr;
			FORMAT Format = FORMAT_UNKNOWN;
		};



		/////////////////////////////////////////////////////////////
		class IDepthStencilView
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