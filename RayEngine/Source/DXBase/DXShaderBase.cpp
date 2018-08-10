#include "..\..\Include\Utilities\EngineUtilities.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DXBase\DXShaderBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DXShaderBase::DXShaderBase()
			: m_ShaderBlob(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN)
		{
		}



		/////////////////////////////////////////////////////////////
		DXShaderBase::~DXShaderBase()
		{
			D3DRelease_S(m_ShaderBlob);
		}



		/////////////////////////////////////////////////////////////
		bool DXShaderBase::CompileFromFile(const std::string& filename, const std::string& filepath, const std::string& entryPoint, SHADER_TYPE type, int32 flags, std::string& errorString)
		{
			std::string source = ReadFullFile(filename, filepath);
			if (source == "")
				return false;

			return CompileFromString(source, entryPoint, type, flags, errorString);
		}



		/////////////////////////////////////////////////////////////
		bool DXShaderBase::CompileFromString(const std::string& src, const std::string& entryPoint, SHADER_TYPE type, int32 flags, std::string& errorString)
		{
			using namespace Microsoft::WRL;

			ComPtr<ID3DBlob> error;
			if (FAILED(D3DCompile(src.c_str(), src.size(), 0, nullptr, nullptr, entryPoint.c_str(),
				GetShaderModel(type).c_str(), flags, 0, &m_ShaderBlob, &error)))
			{
				errorString = reinterpret_cast<const char*>(error->GetBufferPointer());
				return false;
			}
			else
			{
				m_Type = type;
			}

			return true;
		}



		/////////////////////////////////////////////////////////////
		ID3DBlob* DXShaderBase::GetBlob() const
		{
			return m_ShaderBlob;
		}



		/////////////////////////////////////////////////////////////
		std::string DXShaderBase::GetShaderModel(SHADER_TYPE shaderType)
		{
			switch (shaderType)
			{
			case SHADER_TYPE_VERTEX: return "vs_5_0";
			case SHADER_TYPE_HULL: return "hs_5_0";
			case SHADER_TYPE_DOMAIN: return "ds_5_0";
			case SHADER_TYPE_GEOMETRY: return "gs_5_0";
			case SHADER_TYPE_PIXEL: return "ps_5_0";
			case SHADER_TYPE_COMPUTE: return "cs_5_0";
			}

			return std::string();
		}
	}
}

#endif