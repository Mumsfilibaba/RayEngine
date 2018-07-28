#pragma once

#include "..\System\Window.h"
#include "ICommandQueue.h"
#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IFactory;



		/////////////////////////////////////////////////////////////
		struct SwapchainInfo
		{
			System::Window* pWindow = nullptr;
			
			ICommandQueue* pCommandQueue = nullptr;

			struct
			{
				FORMAT Format = FORMAT_UNKNOWN;
				int32 Width = 0;
				int32 Height = 0;
				int32 Count = 0;
			} Buffer;
		};



		/////////////////////////////////////////////////////////////
		class ISwapchain : public IReferenceCounter
		{
		public:
			ISwapchain(ISwapchain&& other) = delete;
			ISwapchain(const ISwapchain& other) = delete;
			ISwapchain& operator=(ISwapchain&& other) = delete;
			ISwapchain& operator=(const ISwapchain& other) = delete;

		public:
			ISwapchain() {}
			virtual ~ISwapchain() {}

			//Returns the number of the current buffer being done rendering
			virtual int32 GetCurrentBuffer() const = 0;
			//Get the texture resource in the chain
			virtual ITexture* GetBuffer(int32 index) = 0;
			virtual const ITexture* GetBuffer(int32 index) const = 0;
			//Present next backbuffer to the front
			virtual void Present() const = 0;
			//Get the CommandQueue that created the swapchain
			virtual ICommandQueue* GetCommandQueue() const = 0;
			//Get the factory that created the swapchain 
			virtual IFactory* GetFactory() const = 0;
		};
	}
}