#include "..\..\Include\DX12\DX12Shader.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Shader::DX12Shader(const ShaderByteCode& bytecode)
			: m_Type(bytecode.GetType()),
			m_Shader()
		{
			m_Shader.BytecodeLength = bytecode.GetSize();

			int8* bytes = new int8[m_Shader.BytecodeLength];
			memcpy(bytes, bytecode.GetBytes(), m_Shader.BytecodeLength);

			m_Shader.pShaderBytecode = bytes;
		}



		/////////////////////////////////////////////////////////////
		DX12Shader::~DX12Shader()
		{
			if (m_Shader.pShaderBytecode != nullptr)
			{
				delete[] m_Shader.pShaderBytecode;
				m_Shader.pShaderBytecode = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		SHADERTYPE DX12Shader::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		const D3D12_SHADER_BYTECODE* DX12Shader::GetD3D12ByteCode() const
		{
			return &m_Shader;
		}
		IReferenceCounter * DX12Shader::QueryReference()
		{
			return nullptr;
		}
		uint32 DX12Shader::GetReferenceCount() const
		{
			return uint32();
		}
		void DX12Shader::Release() const
		{
		}
		uint32 DX12Shader::AddRef()
		{
			return uint32();
		}
	}
}