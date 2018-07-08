#pragma once

#include "..\System\Window.h"

namespace RayEngine
{
	namespace Graphics
	{
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

		class ISwapchain
		{
		public:
			virtual ~ISwapchain() {}

			//Present next backbuffer to the front
			virtual void Present() const = 0;
		};
	}
}