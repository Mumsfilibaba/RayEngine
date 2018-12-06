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

#include "../../Include/Debug/Debug.h"
#include "../../Include/OpenGL/GLDevice.h"
#include "../../Include/OpenGL/GLDeviceContext.h"
#include "../../Include/OpenGL/GLShader.h"
#include "../../Include/OpenGL/GLRootLayout.h"
#include "../../Include/OpenGL/GLPipelineState.h"
#include "../../Include/OpenGL/GLBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Win32\WndclassCache.h"
#endif

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDevice::GLDevice(const DeviceDesc* pDesc)
			: m_ImmediateContext(nullptr),			
			m_NativeContext(RE_GL_NULL_NATIVE_CONTEXT),
			m_Desc(),
			m_References(0),
			m_Extensions()
		{
			AddRef();

			m_Desc = *pDesc;
			Create();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDevice::~GLDevice()
		{
			ReRelease_S(m_ImmediateContext);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::GetImmediateContext(IDeviceContext** ppContext)
		{
			return (*ppContext = m_ImmediateContext->QueryReference<GLDeviceContext>());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return (new GLDeviceContext(this, true));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateShader(IShader** ppShader, const ShaderDesc* pDesc)
		{
			return (*ppShader = new GLShader(this, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			return (*ppBuffer = new GLBuffer(this, pInitialData, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc)
		{
			return (*ppRootLayout = new GLRootLayout(this, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GLDevice::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc)
		{
			return (*ppPipelineState = new GLPipelineState(this, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::GetAdapterDesc(AdapterDesc* pDesc) const
		{
			pDesc->VendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
			pDesc->ModelName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			pDesc->DeviceID = -1;
			pDesc->VendorID = -1;
			pDesc->Flags = ADAPTER_FLAGS_SWAPCHAIN | ADAPTER_FLAGS_GRAPHICS;

			if (ExtensionSupported("GL_ARB_compute_shader"))
				pDesc->Flags |= ADAPTER_FLAGS_COMPUTE;
			if (ExtensionSupported("GL_ARB_geometry_shader4"))
				pDesc->Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			if (ExtensionSupported("GL_ARB_tessellation_shader"))
				pDesc->Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;

			int32 info = 0;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info);
			pDesc->Limits.Texture1D.Width = info;

			pDesc->Limits.Texture2D.Width = info;
			pDesc->Limits.Texture2D.Height = info;

			glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &info);
			pDesc->Limits.Texture3D.Width = info;
			pDesc->Limits.Texture3D.Height = info;
			pDesc->Limits.Texture3D.Depth = info;

			glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &info);
			pDesc->Limits.TextureCube.Width = info;
			pDesc->Limits.TextureCube.Height = info;

			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &info);
			pDesc->Limits.RenderTargetCount = info;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::Release()
		{
			IObject::CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLDevice::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDevice::Create()
		{
			m_ImmediateContext = new GLDeviceContext(this, false);
		}
	}
}