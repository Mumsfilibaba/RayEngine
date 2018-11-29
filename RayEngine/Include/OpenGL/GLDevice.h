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
#include "../Graphics/IDevice.h"
#include "../System/Window.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDeviceContext;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDevice : public IDevice
		{
			RE_IMPLEMENT_INTERFACE(GLDevice);

		public:
			GLDevice(const DeviceDesc* pDesc);
			virtual ~GLDevice();

			inline GLNativeContext GetGLNativeContext() const
			{
				return m_NativeContext;
			}

			inline bool ExtensionSupported(const std::string& extension) const
			{
				for (int32 i = 0; i < static_cast<int32>(m_Extensions.size()); i++)
				{
					if (extension == m_Extensions[i])
						return true;
				}

				return false;
			}

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderDesc* pDesc) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc) override final;

			void SetName(const std::string& name) override final;

			void GetDesc(DeviceDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create();

		protected:
			GLDeviceContext* m_ImmediateContext;
			
			GLNativeContext m_NativeContext;

			DeviceDesc m_Desc;
			
			IObject::CounterType m_References;

			std::vector<std::string> m_Extensions;
		};
	}
}