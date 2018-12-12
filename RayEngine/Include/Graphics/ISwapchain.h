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
#include "Interfaces/IObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class IRenderTargetView;


		/*////////////////////////////////////////////////////////////

			A structure containg info about a swapchain.

			Name - Name of the swapchain.
			s
			Samples - Number of multisampling samples the buffers
					  will use.

			Count - The number of buffers in the swapchain.

			Format - Format of the buffers to use.

			Width - The width of the buffers.

			Height - The height of the buffers.

		////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API SwapchainDesc
		{
			int32 Width;
			int32 Height;
			int32 SampleCount;
			struct
			{
				FORMAT Format;
				int32 Count;
			} BackBuffer;

			struct
			{
				FORMAT Format;
			} DepthStencil;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API ISwapchain : public IObject
		{
			RE_INTERFACE(ISwapchain);

		public:
			ISwapchain() {}
			~ISwapchain() {}

			/*////////////////////////////////////////////////////////////
				
				Resize the size of the framebuffer (Backbuffer and 
				DepthStencil) of the window that were specified when 
				created the swapchain.

				width - The new width of the framebuffer.

				height - The new width of the framebuffer.

			////////////////////////////////////////////////////////////*/
			virtual void Resize(int32 width, int32 height) = 0;

			/*////////////////////////////////////////////////////////////
				Presents and swaps the buffers.
			////////////////////////////////////////////////////////////*/
			virtual void Present() const = 0;

			/*////////////////////////////////////////////////////////////
				
				Queries the device that were used to create the 
				swapchain.

				ppDevice - A valid pointer to an IDevice interface

			////////////////////////////////////////////////////////////*/
			virtual void QueryDevice(IDevice** ppDevice) const = 0;

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(SwapchainDesc* pDesc) const = 0;
		};
	}
}