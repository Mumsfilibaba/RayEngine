#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct RenderTargetViewInfo
		{
			std::string Name = "";
			const ITexture* Resource = nullptr;
			FORMAT Format = FORMAT_UNKNOWN;
		};



		/////////////////////////////////////////////////////////////
		class IRenderTargetView
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