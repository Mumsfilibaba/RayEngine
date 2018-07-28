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
			m_Shader(),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			m_Shader.BytecodeLength = bytecode.GetSize();

			int8* bytes = new int8[m_Shader.BytecodeLength];
			memcpy(bytes, bytecode.GetBytes(), m_Shader.BytecodeLength);

			m_Shader.pShaderBytecode = bytes;
		}




		/////////////////////////////////////////////////////////////
		DX12Shader::DX12Shader(DX12Shader&& other)
			: m_Device(other.m_Device),
			m_Type(other.m_Type),
			m_Shader(other.m_Shader),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_Type = SHADER_TYPE_UNKNOWN;
			other.m_ReferenceCount = 0;

			other.m_Shader.BytecodeLength = 0;
			other.m_Shader.pShaderBytecode = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12Shader::~DX12Shader()
		{
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}


			if (m_Shader.pShaderBytecode != nullptr)
			{
				delete[] m_Shader.pShaderBytecode;
				m_Shader.pShaderBytecode = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX12Shader& DX12Shader::operator=(DX12Shader&& other)
		{
			if (this != &other)
			{
				if (m_Shader.pShaderBytecode != nullptr)
				{
					delete m_Shader.pShaderBytecode;
					m_Shader.pShaderBytecode = nullptr;
				}


				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}

				m_Device = other.m_Device;
				m_Type = other.m_Type;
				m_Shader = other.m_Shader;
				m_ReferenceCount = other.m_ReferenceCount;

				other.m_Device = nullptr;
				other.m_Type = SHADER_TYPE_UNKNOWN;
				other.m_ReferenceCount = 0;
				
				other.m_Shader.BytecodeLength = 0;
				other.m_Shader.pShaderBytecode = nullptr;
			}

			return *this;
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



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12Shader::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Shader::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Shader::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Shader::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}
	}
}

#endif