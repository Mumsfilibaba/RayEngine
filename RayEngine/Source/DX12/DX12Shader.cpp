#include "..\..\Include\DX12\DX12Shader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Shader::DX12Shader(IDevice* pDevice, const ShaderInfo& info)
			: DXShaderBase(),
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12Shader::~DX12Shader()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		SHADER_TYPE DX12Shader::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		void DX12Shader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12Shader::Create(const ShaderInfo& info)
		{
			using namespace System;

			int32 flags = 0;
			if (info.Flags & SHADER_FLAGS_DEBUG)
				flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

			bool result = false;
			std::string errorString;

			if (info.FilePath.size() < 2)
				result = CompileFromString(info.Source, info.EntryPoint, info.Type, flags, errorString);
			else
				result = CompileFromFile(info.Source, info.FilePath, info.EntryPoint, info.Type, flags, errorString);

			if (!result)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not compile shader" + errorString);
				return;
			}
			

			for (int32 i = 0; i < info.SamplerCount; i++)
				m_StaticSamplers.push_back(CreateSampler(info.pSamplers[i]));


			for (int32 i = 0; i < info.VariableCount; i++)
				m_Variables.push_back(CreateVariable(info.pVariables[i]));
		}



		/////////////////////////////////////////////////////////////
		D3D12_STATIC_SAMPLER_DESC DX12Shader::CreateSampler(const StaticSampler& sampler)
		{
			
		}



		/////////////////////////////////////////////////////////////
		D3D12_ROOT_PARAMETER1 DX12Shader::CreateVariable(const ShaderVariable& variable)
		{
			
		}



		/////////////////////////////////////////////////////////////
		D3D12_SHADER_BYTECODE DX12Shader::GetD3D12ByteCode() const
		{
			D3D12_SHADER_BYTECODE byteCode = {};
			byteCode.pShaderBytecode = m_ShaderBlob->GetBufferPointer();
			byteCode.BytecodeLength = m_ShaderBlob->GetBufferSize();
			
			return byteCode;
		}



		/////////////////////////////////////////////////////////////
		const D3D12_STATIC_SAMPLER_DESC* DX12Shader::GetStaticSamplers() const
		{
			return m_StaticSamplers.data();
		}



		/////////////////////////////////////////////////////////////
		int32 DX12Shader::GetStaticSamplerCount() const
		{
			return static_cast<int32>(m_StaticSamplers.size());
		}



		/////////////////////////////////////////////////////////////
		const D3D12_ROOT_PARAMETER1* DX12Shader::GetVariables() const
		{
			return m_Variables.data();
		}



		/////////////////////////////////////////////////////////////
		int32 DX12Shader::GetVariableCount() const
		{
			return static_cast<int32>(m_Variables.size());
		}
	}
}

#endif