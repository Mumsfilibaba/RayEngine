#pragma once

#include "ICommandQueue.h"
#include "IShader.h"
#include "IRenderTargetView.h"
#include "IDepthStencilView.h"
#include "IRootSignature.h"
#include "IPipelineState.h"
#include "IBuffer.h"
#include "AdapterInfo.h"
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
		};



		/////////////////////////////////////////////////////////////
		class IDevice : public RefCounter
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
			virtual bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) = 0;
			//Creates a shader module for use in a pipelinestate
			virtual bool CreateShader(IShader** ppShader, const ShaderInfo& info) = 0;
			//Creates a rendertargetview for using a texture as a rendertarget
			virtual bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) = 0;
			//Creates a rendertargetview for using a texture as a rendertarget
			virtual bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) = 0;
			//Creates a texture resource on the GPU
			virtual bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) = 0;
			//Creates a buffer resource on the GPU
			virtual bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) = 0;
			//Creates a shader rootsignature
			virtual bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) = 0;
			//Create a pipelinestate
			virtual bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) = 0;
			//Gets the device log
			virtual System::Log* GetDeviceLog() = 0;
			//Gets the factory that created the device
			virtual IFactory* GetFactory() const = 0;
		};
	}
}