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

#include "GLSwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "GLDeviceWin32.h"

namespace RayEngine
{
	namespace Graphics
	{
		class GLSwapchainWin32 final : public GLSwapchain
		{
			RE_IMPLEMENT_INTERFACE(GLSwapchainWin32);

		public:
			GLSwapchainWin32(const SwapchainDesc* pDesc, GLDeviceWin32* pDevice);
			~GLSwapchainWin32();

			void MakeCurrent() const override final;

			void Present() const override final;

		private:
			const GLDeviceWin32* m_pDevice;
			HDC m_HDC;
		};
	}
}

#endif