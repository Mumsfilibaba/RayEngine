#pragma once

#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct RenderTargetViewInfo
		{
			std::string Name = "";
			const ITexture* pResource = nullptr;
			FORMAT Format = FORMAT_UNKNOWN;
		};



		/////////////////////////////////////////////////////////////
		class IRenderTargetView : public IReferenceCounter
		{
		public:
			IRenderTargetView(IRenderTargetView&& other) = delete;
			IRenderTargetView(const IRenderTargetView& other) = delete;
			IRenderTargetView& operator=(IRenderTargetView&& other) = delete;
			IRenderTargetView& operator=(const IRenderTargetView& other) = delete;

		public:
			IRenderTargetView() {}
			virtual ~IRenderTargetView() {}

			//Get the device that created the view
			virtual IDevice* GetDevice() const = 0;
		};
	}
}