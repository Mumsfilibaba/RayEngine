#include "..\..\Include\DX12\DX12Shader.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Shader::DX12Shader(IDevice* pDevice, const ShaderByteCode& bytecode)
			: m_Device(nullptr),
			m_Type(bytecode.GetType()),
			m_Shader()
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			m_Shader.BytecodeLength = bytecode.GetSize();

			int8* bytes = new int8[m_Shader.BytecodeLength];
			memcpy(bytes, bytecode.GetBytes(), m_Shader.BytecodeLength);

			m_Shader.pShaderBytecode = bytes;
		}



		/////////////////////////////////////////////////////////////
		DX12Shader::~DX12Shader()
		{
			ReRelease_S(m_Device);

			if (m_Shader.pShaderBytecode != nullptr)
			{
				delete[] m_Shader.pShaderBytecode;
				m_Shader.pShaderBytecode = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		SHADER_TYPE DX12Shader::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12Shader::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		const D3D12_SHADER_BYTECODE* DX12Shader::GetD3D12ByteCode() const
		{
			return &m_Shader;
		}
	}
}

#endif