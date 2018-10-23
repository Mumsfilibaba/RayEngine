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
#include <vector>
#include "..\..\Include\Graphics\ISwapchain.h"
#include "GLCommon.h"
#include "GLDeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLFactory;
		class GLDevice;
		class GLDeviceContext;
		class GLTexture;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLSwapchain final : public ISwapchain
		{
			RE_IMPLEMENT_INTERFACE(GLSwapchain);

		public:
			GLSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainDesc* pDesc);
			~GLSwapchain();

			void Resize(int32 width, int32 height) override final;

			void Present() const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create();

		private:
			GLDevice * m_Device;
			GLFactory* m_Factory;
			
			System::NativeWindowHandle m_WndHandle;
			GLNativeDevice m_NativeDevice;
			
			IObject::CounterType m_References;
		};
	}
}