/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include "..\System\Window.h"
#include "..\ReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IFactory;
		class ITexture;
		class IDevice;



		/*////////////////////////////////////////////////////////////

			A structure containg info about a swapchain.

			Name - Name of the swapchain.

			pWindow - Window for the swapchain to display at when
			calling present.

			Count - The number of buffers in the swapchain.

			Format - Format of the buffers to use.

			Width - The width of the buffers.

			Height - The height of the buffers.

		////////////////////////////////////////////////////////////*/
		struct SwapchainInfo
		{
			std::string Name = "";
			System::NativeWindowHandle Window = RE_NULL_WINDOW;

			int32 Count = 0;
			struct
			{
				FORMAT Format = FORMAT_UNKNOWN;
				int32 Width = 0;
				int32 Height = 0;
			} Buffer;
		};



		/////////////////////////////////////////////////////////////
		class ISwapchain : public ReferenceCounter
		{
		public:
			ISwapchain(ISwapchain&& other) = delete;
			ISwapchain(const ISwapchain& other) = delete;
			ISwapchain& operator=(ISwapchain&& other) = delete;
			ISwapchain& operator=(const ISwapchain& other) = delete;

		public:
			ISwapchain() {}
			virtual ~ISwapchain() {}


			/*////////////////////////////////////////////////////////////
				Returns the index of the current buffer.
			////////////////////////////////////////////////////////////*/
			virtual int32 GetCurrentBuffer() const = 0;


			/*////////////////////////////////////////////////////////////
				Returns the index of the current view.
			////////////////////////////////////////////////////////////*/
			virtual IRenderTargetView* GetCurrentView() const = 0;


			/*////////////////////////////////////////////////////////////
				
				Returns the buffer with a certain index.

				index - Index of the buffer.

			////////////////////////////////////////////////////////////*/
			virtual ITexture* GetBuffer(int32 index) = 0;
			virtual const ITexture* GetBuffer(int32 index) const = 0;


			/*////////////////////////////////////////////////////////////

				Returns the rendertargetview with a certain index.

				index - Index of the buffer.

			////////////////////////////////////////////////////////////*/
			virtual IRenderTargetView* GetRenderTargetView(int32 index) = 0;
			virtual const IRenderTargetView* GetRenderTargetView(int32 index) const = 0;


			/*////////////////////////////////////////////////////////////
				Presents and swaps the buffers.
			////////////////////////////////////////////////////////////*/
			virtual void Present() const = 0;


			/*////////////////////////////////////////////////////////////

				Sets the name of the object.

				name - A new name of the object.

			////////////////////////////////////////////////////////////*/
			virtual void SetName(const std::string& name) = 0;
			

			/*////////////////////////////////////////////////////////////
				
				Queries the device that were used to create the 
				swapchain.

				ppDevice - A valid pointer to an IDevice interface

			////////////////////////////////////////////////////////////*/
			virtual void QueryDevice(IDevice** ppDevice) const = 0;
			

			/*////////////////////////////////////////////////////////////

				Queries the factory that were used to create the
				swapchain.

				ppFactory- A valid pointer to an IFactory interface

			////////////////////////////////////////////////////////////*/
			virtual void QueryFactory(IFactory** ppFactory) const = 0;
		};
	}
}