#pragma once

#include "..\System\Window.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct SwapchainInfo
		{
			System::Window* Window = nullptr;
			struct
			{
				FORMAT Format = FORMAT_UNKNOWN;
				int32 Width = 0;
				int32 Height = 0;
				int32 Count = 0;
			} Buffer;
		};



		/////////////////////////////////////////////////////////////
		class ISwapchain
		{
		public:
			ISwapchain(ISwapchain&& other) = delete;
			ISwapchain(const ISwapchain& other) = delete;
			ISwapchain& operator=(ISwapchain&& other) = delete;
			ISwapchain& operator=(const ISwapchain& other) = delete;

		public:
			ISwapchain() {}
			virtual ~ISwapchain() {}

			//Present next backbuffer to the front
			virtual void Present() const = 0;
		};
	}
}