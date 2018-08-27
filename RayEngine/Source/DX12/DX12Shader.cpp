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
		void DX12Shader::SetName(const std::string& name)
		{
			//Not relevant
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
		}



		/////////////////////////////////////////////////////////////
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