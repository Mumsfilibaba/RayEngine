#pragma once

#include "ICommandQueue.h"
#include "IDeviceContext.h"
#include "IShader.h"
#include "IRenderTargetView.h"
#include "IDepthStencilView.h"
#include "IShaderResourceView.h"
#include "IUnorderedAccessView.h"
#include "IRootLayout.h"
#include "IPipelineState.h"
#include "IBuffer.h"
#include "AdapterInfo.h"
#include "..\IReferenceCounter.h"
#include "..\System\Log.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IFactory;



		/////////////////////////////////////////////////////////////
		struct DeviceInfo
		{
			std::string Name = "";
			AdapterInfo* pAdapter = nullptr;
			int32 DepthStencilDescriptors = 16;
			int32 RendertargetDescriptors = 64;
			int32 ResourceDescriptors = 128;
			int32 SamplerDescriptors = 128;
		};



		/////////////////////////////////////////////////////////////
		class IDevice : public IReferenceCounter
		{
		public:
			IDevice(IDevice&& other) = delete;
			IDevice(const IDevice& other) = delete;
			IDevice& operator=(IDevice&& other) = delete;
			IDevice& operator=(const IDevice& other) = delete;

		public:
			IDevice() {}
			virtual ~IDevice() {}

			//Gets the immediate context
			virtual bool GetImmediateContext(IDeviceContext** ppContext) = 0;
			//Creates an deffered context
			virtual bool CreateDefferedContext(IDeviceContext** ppContext) = 0;
			//Creates a shader module for use in a pipelinestate
			virtual bool CreateShader(IShader** ppShader, const ShaderInfo& info) = 0;
			//Creates a rendertargetview for using a resource as a rendertarget
			virtual bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) = 0;
			//Creates a depthstencilview for using a resource as a depthstencil
			virtual bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) = 0;
			//Creates a shaderresourceview for using a resource as a shaderresource
			virtual bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewInfo& info) = 0;
			//Creates a unorderedaccessview for using a resource in a compute as an unordered resource
			virtual bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewInfo& info) = 0;
			//Creates a sampler
			virtual bool CreateSampler(ISampler** ppSampler, const SamplerInfo& info) = 0;
			//Creates a texture resource on the GPU
			virtual bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) = 0;
			//Creates a buffer resource on the GPU
			virtual bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) = 0;
			//Creates a shader rootsignature
			virtual bool CreateRootLayout(IRootLayout** ppRootSignature, const RootLayoutInfo& info) = 0;
			//Create a pipelinestate
			virtual bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) = 0;
			//Gets the device log
			virtual System::Log* GetDeviceLog() = 0;
			//Gets the factory that created the device
			virtual void QueryFactory(IFactory** ppFactory) const = 0;
		};
	}
}