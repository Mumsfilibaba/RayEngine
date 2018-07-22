#pragma once

#include "ICommandQueue.h"
#include "IFence.h"
#include "IShader.h"
#include "IRenderTargetView.h"
#include "IDepthStencilView.h"
#include "IRootSignature.h"
#include "IPipelineState.h"
#include "IBuffer.h"
#include "AdapterInfo.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct DeviceInfo
		{
			AdapterInfo* pAdapter = nullptr;
		};



		/////////////////////////////////////////////////////////////
		class IDevice
		{
		public:
			IDevice(IDevice&& other) = delete;
			IDevice(const IDevice& other) = delete;
			IDevice& operator=(IDevice&& other) = delete;
			IDevice& operator=(const IDevice& other) = delete;

		public:
			IDevice() {}
			virtual ~IDevice() {}

			//Create a queue for graphics- and computecommands
			virtual bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommanQueueInfo& info) const = 0;
			//Create a fence for synchronization between CPU and GPU
			virtual bool CreateFence(IFence** ppFence) const = 0;
			//Creates a shader module for use in a pipelinestate
			virtual bool CreateShader(IShader** ppShader, const ShaderByteCode& byteCode) const = 0;
			//Creates a rendertargetview for using a texture as a rendertarget
			virtual bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) const = 0;
			//Creates a rendertargetview for using a texture as a rendertarget
			virtual bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) const = 0;
			//Creates a texture resource on the GPU
			virtual bool CreateTexture(ITexture** ppTexture, const TextureInfo& info) const = 0;
			//Creates a shader rootsignature
			virtual bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const = 0;
			//Create a pipelinestate
			virtual bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const = 0;
		};
	}
}