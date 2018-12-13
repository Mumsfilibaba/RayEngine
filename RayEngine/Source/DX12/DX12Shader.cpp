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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Device.h>
#include <DX12/DX12Shader.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Shader::DX12Shader(IDevice* pDevice, const ShaderDesc* pDesc)
			: DXShaderBase(),
			m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Shader::~DX12Shader()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Shader::GetDesc(ShaderDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Shader::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Shader::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Shader::Create(const ShaderDesc* pDesc)
		{
			int32 flags = 0;
			if (pDesc->Flags & SHADER_FLAGS_DEBUG)
				flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

			std::string errorString;
			if (!CompileFromString(pDesc->pSource, pDesc->pEntryPoint, pDesc->Type, flags, errorString))
			{
				LOG_ERROR("D3D12: Could not compile shader.\n" + errorString);
			}
			else
			{
				m_Desc = *pDesc;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_SHADER_BYTECODE DX12Shader::GetD3D12ByteCode() const
		{
			D3D12_SHADER_BYTECODE byteCode = {};
			byteCode.pShaderBytecode = m_ShaderBlob->GetBufferPointer();
			byteCode.BytecodeLength = m_ShaderBlob->GetBufferSize();
			
			return byteCode;
		}
	}
}

#endif