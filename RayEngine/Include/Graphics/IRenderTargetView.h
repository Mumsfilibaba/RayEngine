#pragma once

#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class ITexture;



		/////////////////////////////////////////////////////////////
		struct RenderTargetViewInfo
		{
			std::string Name = "";
			const ITexture* pResource = nullptr;
			FORMAT Format = FORMAT_UNKNOWN;
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