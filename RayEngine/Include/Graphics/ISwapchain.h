#pragma once

#include "..\System\Window.h"
#include "ICommandQueue.h"
#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct SwapchainInfo
		{
			System::Window* Window = nullptr;
			
			ICommandQueue* commandQueue = nullptr;

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

			//Get the texture resource in the chain
			virtual const ITexture* GetBuffer(int32 index) const = 0;
			//Present next backbuffer to the front
			virtual void Present() const = 0;
		};
	}
}