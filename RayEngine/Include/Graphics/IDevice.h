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
#include "AdapterDesc.h"
#include "../Interfaces/IObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IFactory;
		class IDeviceContext;
		class IShader;
		struct ShaderDesc;

		class IRenderTargetView;
		struct RenderTargetViewDesc;

		class IDepthStencilView;
		struct DepthStencilViewDesc;
		
		class IShaderResourceView;
		struct ShaderResourceViewDesc;

		class IUnorderedAccessView;
		struct UnorderedAccessViewDesc;

		class IBuffer;
		struct BufferDesc;

		class IRootLayout;
		struct RootLayoutDesc;

		class IPipelineState;
		struct PipelineStateDesc;

		class ISampler;
		struct SamplerDesc;

		class ITexture;
		struct TextureDesc;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum DEVICE_FLAG : int32
		{
			DEVICE_FLAG_NONE = 0,
			DEVICE_FLAG_DEBUG = (1 << 0),
		};


		/*////////////////////////////////////////////////////////////
			
			Contains info about a logical device. Used to create
			a device interface.

			Name - Name of the device. Used for debugging.

			pAdapter - Pointer to the adapter (physcial device) 
			that should be used to create the logical device.
			If set to nullptr the default adapter will be used,
			which most often is the first adapter found on the
			system.

			DepthStencilDescriptorCount - Number of DepthStencilViews
			that can be created by the device. For apis that uses
			descriptor heaps this is used to set the number of
			descriptor in the heap.

			RendertargetDescriptorCount - Number of RenderTargetViews
			that can be created by the device.
			
			ResourceDescriptorCount- Number of ShaderResourceViews,
			UnorderedAccessViews and ConstantBuffers that can be created
			by the device. Resources has one shared descriptorheap
			so this number is recommended to be higher.

			SamplerDescriptorCount - Number of samplers that can
			be created by the system.

		////////////////////////////////////////////////////////////*/
		struct DeviceDesc
		{
			std::string Name;
			int32 DeviceFlags;
			int32 DepthStencilDescriptorCount;
			int32 RendertargetDescriptorCount;
			int32 ResourceDescriptorCount;
			int32 SamplerDescriptorCount;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice : public IObject
		{
			RE_INTERFACE(IDevice);

		public:
			IDevice() {}
			~IDevice() {}

			/*////////////////////////////////////////////////////////////
				
				Gets the immediate context connected to the device.
				Immediete contexts are used to extecute deffered contexts
				and CPU and GPU are automaticlly synced. Release needs 
				to be called on the returned context.

				ppContext - A valid pointer to an IDeviceContext interface.

				Returns true if ppContext is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool GetImmediateContext(IDeviceContext** ppContext) = 0;

			/*////////////////////////////////////////////////////////////

				Creates a new context that is deffered. No commands are
				executed since after ExecuteDefferedContext has been called
				on a immediate context. A deffered context is used to store
				commands that can be executed later. No GPU-state that is 
				changed on a deffered context is set until
				ExecuteDefferedContext has been called. Release needs
				to be called on the returned context.

				ppContext - A valid pointer to an IDeviceContext interface.

				Returns true if ppContext is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateDefferedContext(IDeviceContext** ppContext) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates and compiles a new ShaderObject. Release needs
				to be called on the returned object.

				ppShader - A valid pointer to an IShader interface.

				info - A structure defining the properties of the 
				shaderobject.
				
				Returns true if ppShader is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateShader(IShader** ppShader, const ShaderDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new RenderTargetView. Release needs to be 
				called on the returned object.

				ppView - A valid pointer to an IRenderTargetView
				interface.

				info - A structure defining the properties of the
				RenderTargetView.

				Returns true if ppView is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc) = 0;

			/*////////////////////////////////////////////////////////////

				Creates a new DepthStencilView. Release needs to be 
				called on the returned object.

				ppView - A valid pointer to an IDepthStencilView
				interface.

				info - A structure defining the properties of the
				DepthStencilView.

				Returns true if ppView is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new ShaderResourceView. Release needs to be 
				called on the returned object.

				ppView - A valid pointer to an IShaderResourceView
				interface.

				info - A structure defining the properties of the
				ShaderResourceView.

				Returns true if ppView is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new UnorderedAccessView. Release needs to be 
				called on the returned object.

				ppView - A valid pointer to an IUnorderedAccessView
				interface.

				info - A structure defining the properties of the
				UnorderedAccessView.

				Returns true if ppView is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new Sampler. Release needs to be called on 
				the returned object.

				ppSampler - A valid pointer to an ISampler interface.

				info - A structure defining the properties of the Sampler.

				Returns true if ppSampler is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new Texture resource on the GPU. Release needs
				to be called on the returned object.

				ppTexture - A valid pointer to an ITexture interface.

				pInitialData - A structure containing the data that the 
				resource will be initialized with. The data needs to
				match the properties set in the info-structure.

				info - A structure defining the properties of the 
				Texture.

				Returns true if ppTexture is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new Buffer resource on the GPU. Release needs
				to be called on the returned object.

				ppBuffer - A valid pointer to an IBuffer interface.

				pInitialData - A structure containing the data that the
				resource will be initialized with. The data needs to
				match the properties set in the info-structure.

				info - A structure defining the properties of the
				Buffer.

				Returns true if ppBuffer is equal to a valid pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new RootLayout. Release needs to be called
				on the returned object.

				ppRootSignature - A valid pointer to an IRootLayout 
				interface.

				info - A structure defining the properties of the
				RootLayout.

				Returns true if ppRootSignature is equal to a valid
				pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateRootLayout(IRootLayout** ppRootSignature, const RootLayoutDesc* pDesc) = 0;
			
			/*////////////////////////////////////////////////////////////

				Creates a new PipelineState. Release needs to be called
				on the returned object.

				ppPipelineState - A valid pointer to an IPipelineState
				interface.

				info - A structure defining the properties of the
				PipelineState.

				Returns true if ppPipelineState is equal to a valid
				pointer.

			////////////////////////////////////////////////////////////*/
			virtual bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc) = 0;

			
			/*////////////////////////////////////////////////////////////

				Sets the name of the object.

				name - A new name of the object.

			////////////////////////////////////////////////////////////*/
			virtual void SetName(const std::string& name) = 0;

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(DeviceDesc* pDesc) const = 0;
		};
	}
}