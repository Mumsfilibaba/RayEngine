#include "..\..\Include\Graphics\IShaderCompiler.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		ShaderByteCode::ShaderByteCode()
			: m_Lang(SHADER_SOURCE_LANG_UNKNOWN),
			m_Type(SHADER_TYPE_UNKNOWN),
			m_Bytes(nullptr),
			m_Size(0),
			m_EntryPoint()
		{
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode::ShaderByteCode(const std::string& entryPoint, SHADER_TYPE type, SHADER_SOURCE_LANG srcLang, int8* bytes, int32 size)
			: m_Lang(srcLang),
			m_Type(type),
			m_Bytes(nullptr),
			m_Size(size),
			m_EntryPoint(entryPoint)
		{
			m_Bytes = new int8[m_Size];
			memcpy(m_Bytes, bytes, m_Size);
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode::ShaderByteCode(const ShaderByteCode& other)
			: m_Lang(other.m_Lang),
			m_Type(other.m_Type),
			m_Bytes(nullptr),
			m_Size(other.m_Size),
			m_EntryPoint(other.m_EntryPoint)
		{
			m_Bytes = new int8[m_Size];
			memcpy(m_Bytes, other.m_Bytes, m_Size);
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode::ShaderByteCode(ShaderByteCode&& other)
			: m_Lang(other.m_Lang),
			m_Type(other.m_Type),
			m_Bytes(other.m_Bytes),
			m_Size(other.m_Size),
			m_EntryPoint(std::move(other.m_EntryPoint))
		{
			other.m_Bytes = nullptr;
			other.m_Lang = SHADER_SOURCE_LANG_UNKNOWN;
			other.m_Size = 0;
		}
		


		/////////////////////////////////////////////////////////////
		ShaderByteCode::~ShaderByteCode()
		{
			Release();
		}



		/////////////////////////////////////////////////////////////
		int8* ShaderByteCode::Detach()
		{
			int8* bytes = m_Bytes;
			m_Bytes = nullptr;
			m_Type = SHADER_TYPE_UNKNOWN;
			m_Lang = SHADER_SOURCE_LANG_UNKNOWN;
			m_Size = 0;
			m_EntryPoint.clear();

			return bytes;
		}



		/////////////////////////////////////////////////////////////
		SHADER_SOURCE_LANG ShaderByteCode::GetSourceLanguage() const
		{
			return m_Lang;
		}



		/////////////////////////////////////////////////////////////
		SHADER_TYPE ShaderByteCode::GetType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		const int8* ShaderByteCode::GetBytes() const
		{
			return m_Bytes;
		}



		/////////////////////////////////////////////////////////////
		const std::string& ShaderByteCode::GetEntryPoint() const
		{
			return m_EntryPoint;
		}


		/////////////////////////////////////////////////////////////
		int32 ShaderByteCode::GetSize() const
		{
			return m_Size;
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode& ShaderByteCode::operator=(const ShaderByteCode& other)
		{
			if (this != &other)
			{
				Release();

				m_Lang = other.m_Lang;
				m_Type = other.m_Type;
				m_Size = other.m_Size;
				m_EntryPoint = other.m_EntryPoint;

				m_Bytes = new int8[m_Size];
				memcpy(m_Bytes, other.m_Bytes, m_Size);
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode& ShaderByteCode::operator=(ShaderByteCode&& other)
		{
			if (this != &other)
			{
				Release();

				m_Bytes = other.m_Bytes;
				m_Size = other.m_Size;
				m_Lang = other.m_Lang;
				m_Type = other.m_Type;

				other.m_Bytes = nullptr;
				other.m_Size = 0;
				other.m_Lang = SHADER_SOURCE_LANG_UNKNOWN;
				other.m_Type = SHADER_TYPE_UNKNOWN;

				m_EntryPoint.swap(m_EntryPoint);
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void ShaderByteCode::Release()
		{
			if (m_Bytes != nullptr)
			{
				delete[] m_Bytes;
				m_Bytes = nullptr;
			}
		}
	}
}