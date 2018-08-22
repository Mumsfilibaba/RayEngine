#pragma once

#include <vector>
#include "..\Graphics\IShader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"
#include "..\DXBase\DXShaderBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12Shader final : public IShader, public DXShaderBase
		{
		public:
			DX12Shader(const DX12Shader& other) = delete;
			DX12Shader& operator=(const DX12Shader& other) = delete;
			DX12Shader(DX12Shader&& other) = delete;
			DX12Shader& operator=(DX12Shader&& other) = delete;

		public:
			DX12Shader(IDevice* pDevice, const ShaderInfo& info);
			~DX12Shader();

			D3D12_SHADER_BYTECODE GetD3D12ByteCode() const;
			const D3D12_STATIC_SAMPLER_DESC* GetStaticSamplers() const;
			int32 GetStaticSamplerCount() const;
			const D3D12_ROOT_PARAMETER1* GetVariables() const;
			int32 GetVariableCount() const;

			SHADER_TYPE GetType() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ShaderInfo& info);
			D3D12_ROOT_PARAMETER1 CreateVariable(const ShaderVariable& variable);

		private:
			DX12Device* m_Device;
		};
	}
}

#endif